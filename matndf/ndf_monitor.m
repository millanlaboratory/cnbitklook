%   Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>
%
%   This file is part of matndf
%
%   The libndf library is free software: you can redistribute it and/or
%   modify it under the terms of the version 3 of the GNU General Public
%   License as published by the Free Software Foundation.
%
%   This program is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with this program.  If not, see <http://www.gnu.org/licenses/>.
%
%   function ndf_checkonline(pipename, aD, aC)
function ndf_checkonline(pn, aD, aC)
% Include all the required toolboxes
ndf_include();

% Prepare and enter main loop
try 
	% Prepare Loop structure
	loop.cl   = ndf_cl();
	loop.jump = ndf_jump();
	
	% Check the names
	% - if in format /name, then query the nameserver for the IPs/filenames
	% - otherwise, keep them as they are
	% - if pn, aD or aC are empty after calling ndf_checknames, their value
	%   will be set according to what is stored in the XML configuration
	% - also, if the nameserver query fails, pn, aD and aC will be empty and
	%   their values will be set according to the XML configuration
	%
	% 2011-05-11  Michele Tavella <michele.tavella@epfl.ch>
	% TODO I believe that the behaviour above might be a bit confusing, 
	% so please feel free to do what you think is best to increase ease of use
	%[pn, aD, aC] = ndf_checknames(loop.cl, pn, aD, aC);
	[pn, aD, aC] = ndf_checknames(loop.cl, pn, aD, aC);
	
	% Get the configuration
	cfg = ndf_cl_getconfig(loop.cl, 'ndfcheck_online');
	
	% If one among pn, aD or aC is empty, then get their values from the
	% configuration and re-check the names
	if(isempty(aC)); aC = cfg.ndf.ic; end
	if(isempty(aD)); aD = cfg.ndf.id; end
	if(isempty(pn)); pn = cfg.ndf.pn; end
	[pn, aD, aC] = ndf_checknames(loop.cl, pn, aD, aC);
	


	% Prepare TOBI structure
	% - when retrieving the taskset from the XML configuration, we also ask 
	%   for an iC message to be returned configured according to the taskset
	% - later on we will also aks for an iD message, but as today this is not
	%   implemented yet
	tobi = ndf_tobi(aD, aC, cfg.messageC);

	% -------------------------------------------------------------- %
	% User initialization                                            %
	% -------------------------------------------------------------- %
	cfg.ns.scope = cl_retrieve(loop.cl, 'ndfcheck_online::scope');
	cfg.scope = strcmp(cfg.ns.scope, 'true');
	cfg.scope = 1;
	% -------------------------------------------------------------- %
	% /User initialization                                           %
	% -------------------------------------------------------------- %

	% Prepare NDF srtructure
	ndf.conf  = {};
	ndf.size  = 0;
	ndf.frame = ndf_frame();
	ndf.sink  = ndf_sink(pn);
		

	% -------------------------------------------------------------- %
	% User TOBI configuration                                        %
	% -------------------------------------------------------------- %
	% Configure TiD message
	idmessage_setdescription(tobi.iD.message, 'ndf_monitor');
	idmessage_setfamilytype(tobi.iD.message, idmessage_familytype('biosig'));
	idmessage_setevent(tobi.iD.message, 0);
	% -------------------------------------------------------------- %
	% /User TOBI configuration                                       %
	% -------------------------------------------------------------- %
	
	% Pipe opening and NDF configurationf
	% - Here the pipe is opened
	% - ... and the NDF ACK frame is received
	disp('[ndf_monitor] Receiving ACK...');
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
	disp(['[ndf_monitor] NDF type id: ' num2str(ndf.conf.id)]);

	% Ring-buffers
	% - By default they contain the last 2 seconds of data
	% - Eventually, save CPU by not buffering unuseful dat
	% - If you want to store the timestamps for debugging, then
	%   also configure buffer.tim in this way:
	%   buffer.tim = ndf_ringbuffer(ndf.conf.sf/ndf.conf.samples, 10);
	%
	disp('[ndf_monitor] Creating ring-buffers');
	buffer.eeg = ndf_ringbuffer(ndf.conf.sf, ndf.conf.eeg_channels, 2.00);
	buffer.exg = ndf_ringbuffer(ndf.conf.sf, ndf.conf.exg_channels, 2.00);
	buffer.tri = ndf_ringbuffer(ndf.conf.sf, ndf.conf.tri_channels, 2.00);

	% Initialize ndf_jump structure
	% - Each NDF frame carries an index number
	% - ndf_jump*.m are methods to verify whether your script is
	%   running too slow
	disp('[ndf_monitor] Receiving NDF frames...');
	loop.jump.tic = ndf_tic();
	while(true) 
		% Read NDF frame from pipe
		loop.jump.toc = ndf_toc(loop.jump.tic);
		loop.jump.tic = ndf_tic();
		[ndf.frame, ndf.size] = ndf_read(ndf.sink, ndf.conf, ndf.frame);

		% Acquisition is down, exit
		if(ndf.size == 0)
			disp('[ndf_monitor] Broken pipe');
			break;
		end
		
		% Buffer NDF streams to the ring-buffers
		% - If you want to keep track of the timestamps for debugging, 
		%   update buffer.tim in this way:
		%	buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(ndf.frame.timestamp));
		buffer.eeg = ndf_add2buffer(buffer.eeg, ndf.frame.eeg);
		buffer.exg = ndf_add2buffer(buffer.exg, ndf.frame.exg);
		buffer.tri = ndf_add2buffer(buffer.tri, ndf.frame.tri);

		% -------------------------------------------------------------- %
		% User main loop                                                 %
		% -------------------------------------------------------------- %
		if(configuration.scope == true)
			eegc3_figure(1);
			subplot(7, 1, 1:4)
			imagesc(eegc3_car(eegc3_dc(buffer.eeg))');
			title(sprintf('Frame=%6.6d, Dl=%7.2f (ms)', ndf.frame.index, loop.jump.toc));
			set(gca, 'XTickLabel', {});
			ylabel('eeg');
			
			subplot(7, 1, 5:6)
			imagesc(eegc3_car(eegc3_dc(buffer.exg))');
			set(gca, 'XTickLabel', {});
			ylabel('exg');
			
			subplot(7, 1, 7)
			imagesc(buffer.tri');
			ylabel('tri');
			set(gca, 'XTickLabel', {});
			set(gca, 'YTickLabel', {});
			drawnow;
		end

		% Connect and send data to endpoint
		% - The idea is that the endpoint might go up/down while this module
		%   follows the acquisition
		% - So, if the endpoint falls, we disconnect the socket and wait for the
		%   endpoint to come up again
		% - If the endpoint is connected, send the current TiC/TiD message
		if(ndf_tobi_connectid(tobi) == true)
			if(mod(ndf.frame.index, 160) == 0)
				idmessage_setevent(tobi.iD.message, ndf.frame.index);
				ndf_tobi_sendd(tobi, ndf.frame.index);
			end

			if(ndf_tobi_receiveid(tobi) > 0)
				for iq = 1:length(tobi.iD.queue)
					idmessage_deserialize(tobi.iD.serializer, tobi.iD.queue{iq});
					idmessage_dump(tobi.iD.message);
				end
			end
		end
		
		% Same as before, for TiC
		if(ndf_tobi_connectic(tobi) == true)
			icmessage_setvalue(tobi.iC.message, ... 
				'ndf_monitor', 'frame', ndf.frame.index);
			ndf_tobi_sendc(tobi, ndf.frame.index);
		end
		% -------------------------------------------------------------- %
		% /User main loop                                                %
		% -------------------------------------------------------------- %

		% Check if module is running slow
		loop.jump = ndf_jump_update(loop.jump, ndf.frame.index);
		if(loop.jump.isjumping)
			disp('[ndf_monitor] Error: running slow');
			break;
		end
	end
catch exception
	disp(['[ndf_monitor] Exception: ' exception.message ]);
	disp(exception);
	disp(exception.stack);
	disp('[ndf_monitor] Killing Matlab...');
	if(exit('ndf.sink', 'var') == 1)
		ndf_close(ndf.sink);
	end
	if(exit('tobi', 'var') == 1)
		ndf_tobi_close(tobi);
	end
	if(exit('loop.cl', 'var') == 1)
		cl_disconnect(loop.cl);
		cl_delete(loop.cl);
	end
	exit;
end

disp('[ndf_monitor] Going down');
ndf_close(ndf.sink);
ndf_tobi_close(tobi);
cl_disconnect(loop.cl);
cl_delete(loop.cl);
