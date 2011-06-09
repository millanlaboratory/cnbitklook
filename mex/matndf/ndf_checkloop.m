% Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
% Michele Tavella <michele.tavella@epfl.ch>
%
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.

function ndf_checkloop(arg0, arg1, arg2)

% For historical reasons this function accepts 3 arguments.
% Normally not needed, but the users might want to pass something
% when launching this function for debugging (and not within the loop)
if(nargin == 0) 
	arg0 = ''; 
	arg1 = ''; 
	arg2 = ''; 
end

% Include all the required toolboxes
ndf_include();

% Prepare and enter main loop
try 
	% Initialize loop structure
	ndf_loopnew;

	% Connect to the CnbiTk loop
	if(cl_connect(loop.cl) == false)
		disp('[ndf_checkloop] Cannot connect to CNBI Loop, killing matlab');
		exit;
	end	

	% Check the names
	% - if in format /name, then query the nameserver for the IPs/filenames
	% - otherwise, keep them as they are
	% - if pn, aD or aC are empty after calling ndf_checknames, their value
	%   will be set according to what is stored in the XML configuration
	% - also, if the nameserver query fails, pn, aD and aC will be empty and
	%   their values will be set according to the XML configuration
	loop = ndf_loopconfig(loop, 'checkloop');
	if(loop.cfg.config == 0)
		disp('[ndf_checkloop] Cannot retrieve loop configuration, killing matlab');
		exit;
	end
	loop = ndf_loopnames(loop);
	
	if(isempty(loop.cfg.ndf.pipe))
		disp('[ndf_checkloop] NDF configuration failed, killing matlab:');
		disp(['  Pipename:   "' loop.cfg.ndf.pipe '"']);
		disp(['  iC address: "' loop.cfg.ndf.ic '"']);
		disp(['  iD address: "' loop.cfg.ndf.id '"']);
		exit;
	end

	% -------------------------------------------------------------- %
	% User initialization                                            %
	% -------------------------------------------------------------- %
	user.ns.plot = cl_retrieveconfig(loop.cl, 'checkloop', 'plot');
	user.plot = strcmp(user.ns.plot, 'true');
	% -------------------------------------------------------------- %
	% /User initialization                                           %
	% -------------------------------------------------------------- %

	% Prepare NDF srtructure
	ndf.conf  = {};
	ndf.size  = 0;
	ndf.frame = ndf_frame();
	ndf.sink  = ndf_sink(loop.cfg.ndf.pipe);

	% -------------------------------------------------------------- %
	% User TOBI configuration                                        %
	% -------------------------------------------------------------- %
	% Configure TiD message
	idmessage_setevent(loop.mDo, 0);
	% Dump TiC/TiD messages
	icmessage_dumpmessage(loop.mC);
	idmessage_dumpmessage(loop.mDo);
	% -------------------------------------------------------------- %
	% /User TOBI configuration                                       %
	% -------------------------------------------------------------- %
	
	% Pipe opening and NDF configurationf
	% - Here the pipe is opened
	% - ... and the NDF ACK frame is received
	disp('[ndf_checkloop] Receiving ACK...');
	[ndf.conf, ndf.size] = ndf_ack(ndf.sink);
	
	% -------------------------------------------------------------- %
	% User EEG data configuration                                    %
	% -------------------------------------------------------------- %
	
	% -------------------------------------------------------------- %
	% /User EEG data configuration                                   %
	% -------------------------------------------------------------- %
	
	% NDF ACK check
	% - The NDF id describes the acquisition module running
	% - Bind your modules to a particular configuration (if needed)
	disp(['[ndf_checkloop] NDF type id: ' num2str(ndf.conf.id)]);

	% Ring-buffers
	% - By default they contain the last 2 seconds of data
	% - Eventually, save CPU by not buffering unuseful dat
	% - If you want to store the timestamps for debugging, then
	%   also configure buffer.tim in this way:
	%   buffer.tim = ndf_ringbuffer(ndf.conf.sf/ndf.conf.samples, 10);
	%
	disp('[ndf_checkloop] Creating ring-buffers');
	buffer.eeg = ndf_ringbuffer(ndf.conf.sf, ndf.conf.eeg_channels, 2.00);
	buffer.exg = ndf_ringbuffer(ndf.conf.sf, ndf.conf.exg_channels, 2.00);
	buffer.tri = ndf_ringbuffer(ndf.conf.sf, ndf.conf.tri_channels, 2.00);
	buffer.tim = ndf_ringbuffer(ndf.conf.sf/ndf.conf.samples, ndf.conf.tim_channels, 5.00);
	
	% Initialize ndf_jump structure
	% - Each NDF frame carries an index number
	% - ndf_jump*.m are methods to verify whether your script is
	%   running too slow
	disp('[ndf_checkloop] Receiving NDF frames...');
	loop.jump.tic = ndf_tic();
	while(true) 
		% Read NDF frame from pipe
		loop.jump.toc = ndf_toc(loop.jump.tic);
		loop.jump.tic = ndf_tic();
		[ndf.frame, ndf.size] = ndf_read(ndf.sink, ndf.conf, ndf.frame);

		% Acquisition is down, exit
		if(ndf.size == 0)
			disp('[ndf_checkloop] Broken pipe');
			break;
		end
		
		% Buffer NDF streams to the ring-buffers
		% - If you want to keep track of the timestamps for debugging, 
		%   update buffer.tim in this way:
		%	buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(ndf.frame.timestamp));
		buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(ndf.frame.timestamp));
		buffer.eeg = ndf_add2buffer(buffer.eeg, ndf.frame.eeg);
		buffer.exg = ndf_add2buffer(buffer.exg, ndf.frame.exg);
		buffer.tri = ndf_add2buffer(buffer.tri, ndf.frame.tri);

		% -------------------------------------------------------------- %
		% User main loop                                                 %
		% -------------------------------------------------------------- %
		if(user.plot == true)
			eegc3_figure(1);
			subplot(10, 1, 1:4)
			imagesc(eegc3_car(eegc3_dc(buffer.eeg))');
			title(sprintf('Frame=%6.6d, Dl=%7.2f (ms)', ndf.frame.index, loop.jump.toc));
			set(gca, 'XTickLabel', {});
			ylabel('eeg');
			
			subplot(10, 1, 5:6)
			imagesc(eegc3_car(eegc3_dc(buffer.exg))');
			set(gca, 'XTickLabel', {});
			ylabel('exg');
			
			subplot(10, 1, 7)
			imagesc(buffer.tri');
			ylabel('tri');
			set(gca, 'XTickLabel', {});
			set(gca, 'YTickLabel', {});
			
			subplot(10, 1, 8:10)
			plot(buffer.tim);
			ylabel('tim');
			set(gca, 'XTickLabel', {});
			xlabel(sprintf('Max delay %.2f ms', max(abs(buffer.tim))));
			axis tight;
			drawnow;
		end

		% Handle async TOBI iD communication
		if(tid_isattached(loop.iD) == true)
			if(mod(ndf.frame.index, 16) == 0)
				idmessage_setevent(loop.mDo, ndf.frame.index);
				tid_setmessage(loop.iD, loop.sDo, ndf.frame.index);
			end
			
			while(tid_getmessage(loop.iD, loop.sDi) == true)
				idmessage_dumpmessage(loop.mDi);
			end
		else
			tid_attach(loop.iD, loop.cfg.ndf.id);
		end
		
		% Handle sync TOBI iC communication
		if(tic_isattached(loop.iC) == true)
			for t = 0:ccfgtaskset_count(loop.cfg.taskset) - 1
				taskname = ccfgtaskset_gettaskbyid(loop.cfg.taskset, t);
				tasklabel = num2str(ccfgtask_getgdf(taskname));
				icmessage_setvalue(loop.mC, loop.cfg.classifier.name, ...
					tasklabel, ndf.frame.index);
			end
			tic_setmessage(loop.iC, loop.sC, ndf.frame.index);
		else
			tic_attach(loop.iC, loop.cfg.ndf.ic);
		end
		% -------------------------------------------------------------- %
		% /User main loop                                                %
		% -------------------------------------------------------------- %

		% Check if module is running slow
		loop.jump = ndf_jump_update(loop.jump, ndf.frame.index);
		if(loop.jump.isjumping)
			disp('[ndf_checkloop] Error: running slow');
			break;
		end
	end
catch exception
	ndf_printexception(exception);
	disp('[ndf_checkloop] Going down');
	loop.exit = true;
end

try 
	% Tear down loop structure
	ndf_loopdelete;
catch exception
	ndf_printexception(exception);
	loop.exit = true;
end

disp(['[ndf_checkloop] Loop uptime: ' ...
	num2str(ndf_toc(loop.uptime)/1000/60) ...
	' minutes']);

disp('[ndf_checkloop] Killing Matlab...');
if(loop.exit == true)
	exit;
end
