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
%   function ndf_monitor(pipename, addressD, addressC)
function ndf_monitor(pipename, addressD, addressC)

if(nargin < 3); addressC = ''; end
if(nargin < 2); addressD = ''; end
if(nargin < 1); pipename = ''; end

% Include all the required toolboxes
ndf_include();

% Prepare and enter main loop
try 
	% Prepare Loop structure
	loop.cl   = {};
	loop.tic  = 0;
	loop.toc  = 0;
	loop.jump = ndf_jump();
	[loop.cl, pipename, addressD, addressC] = ...
		ndf_cl(pipename, addressD, addressC);	
	
	% -------------------------------------------------------------- %
	% User initialization                                            %
	% -------------------------------------------------------------- %
	cfg = {};
	cfg.ns.block   = cl_retrieve(loop.cl, 'ndf_monitor::block');
	cfg.ns.taskset = cl_retrieve(loop.cl, 'ndf_monitor::taskset');
	cfg.ns.xml     = cl_retrieve(loop.cl, 'ndf_monitor::xml');
		
	cfg.taskset = ccfg_onlinem(config, cfg.ns.block, cfg.ns.taskset);

	[cfg.classifier.name, cfg.classifier.description, cfg.classifier.filename] = ...
		ccfgtaskset_getclassifier(cfg.ns.taskset);
	[cfg.ndf.function, cfg.ndf.pipename, cfg.ndf.id, cfg.ndf.ic] = ...
		ccfgtaskset_getndf(cfg.ns.taskset);
	if(isempty(addressC)); addressC = cfg.ndf.ic; end
	if(isempty(addressD)); addressD = cfg.ndf.id; end
	if(isempty(pipename)); pipename = cfg.ndf.pipename; end

	cfg.ns.scope   = cl_retrieve(loop.cl, 'ndf_monitor::scope');
	cfg.scope = strcmp(configuration.ns.scope, 'true');
	% -------------------------------------------------------------- %
	% /User initialization                                           %
	% -------------------------------------------------------------- %

	% Prepare NDF srtructure
	ndf.conf  = {};
	ndf.size  = 0;
	ndf.frame = ndf_frame();
	ndf.sink  = ndf_sink(pipename);
	% Prepare TOBI structure
	tobi      = ndf_tobi(addressD, addressC);
		
	% -------------------------------------------------------------- %
	% User TOBI configuration                                        %
	% -------------------------------------------------------------- %
	% Configure TiD message
	idmessage_setdescription(tobi.iD.message, 'ndf_monitor');
	idmessage_setfamilytype(tobi.iD.message, idmessage_familytype('biosig'));
	idmessage_setevent(tobi.iD.message, 0);

	% Configure TiC message
	icmessage_addclassifier(tobi.iC.message, ...
		'ndf_monitor', ...
		'matndf passive monitor', ...
		icmessage_getvaluetype('prob'), ...
		icmessage_getlabeltype('custom'));
	icmessage_addclass(tobi.iC.message, ...
		'ndf_monitor', ... 
		'frame', ...
		781);
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
	loop.tic = ndf_tic();
	while(true) 
		% Read NDF frame from pipe
		loop.toc = ndf_toc(loop.tic);
		loop.tic = ndf_tic();
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
			title(sprintf('Frame=%6.6d, Dl=%7.2f (ms)', ndf.frame.index, loop.toc));
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
	ndf_close(ndf.sink);
	ndf_tobi_close(tobi);
	cl_disconnect(loop.cl);
	cl_delete(loop.cl);
	exit;
end

disp('[ndf_monitor] Going down');
ndf_close(ndf.sink);
ndf_tobi_close(tobi);
cl_disconnect(loop.cl);
cl_delete(loop.cl);
