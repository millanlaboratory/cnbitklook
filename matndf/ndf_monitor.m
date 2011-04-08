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
%   function config = ndf_monitor(pipename, addressD, addressC)
function config = ndf_monitor(pipename, addressD, addressC)

% Verify if mtpath is installed
% Configuration 
% - Initialize, configure and setup the required toolboxes
try
	if(isempty(which('mtpath_include')))
		addpath('/opt/mtpath');
	end

	if(getenv('CNBITK_MATLAB'))
		mtpath_include('$CNBITK_MATLAB/');
	else
		mtpath_include('/opt/cnbitkmat/');
	end
	if(getenv('EEGC3_ROOT'))
		mtpath_include('$EEGC3_ROOT/');
		mtpath_include('$EEGC3_ROOT/modules/smr');
	else
		mtpath_include('/opt/eegc3');
		mtpath_include('/opt/eegc3/modules/smr');
	end
catch exception
	disp(['[ndf_monitor] Exception: ' exception.message ]);
	disp(exception.stack);
	disp('[ndf_monitor] Killing Matlab...');
	exit;
end

% Prepare and enter main loop
try 
	% Configure TCP endpoints for TiD and TiC
	hostD.socket  = [];
	hostD.address = '127.0.0.1:9000';
	hostD.ipport  = {'127.0.0.1', '9000'};
	hostD.message = idmessage_new();
	hostD.serializer = idserializerrapid_new(hostD.message);
	hostD.buffer = '';
	hostD.cache = '';
	hostD.hdr = '<tobiid';
	hostD.trl = '/>';
	hostD.queue = {};

	hostC.socket  = [];
	hostC.address = '127.0.0.1:9500';
	hostC.ipport  = {'127.0.0.1', '9500'};
	hostC.message = icmessage_new();
	hostC.serializer = icserializerrapid_new(hostC.message);
	hostC.cache = '';
	hostC.hdr = '<tobiic';
	hostC.trl = '</tobiic>';
	
	% Configure TiD message
	idmessage_setdescription(hostD.message, 'ndf_monitor');
	idmessage_setfamilytype(hostD.message, idmessage_familytype('biosig'));
	idmessage_setevent(hostD.message, 0);

	% Configure TiC message
	icmessage_addclassifier(hostC.message, ...
		'ndf_monitor', 'matndf passive monitor', ...
		icmessage_getvaluetype('probs'), icmessage_getlabeltype('custom'));
	icmessage_addclass(hostC.message, 'ndf_monitor', 'frame', 781);

	if(nargin >= 2)
		hostD.address = addressD;
		hostD.ipport = regexp(hostD.address, ':', 'split');
	end
	if(nargin >= 3)
		hostC.address = addressC;
		hostC.ipport = regexp(hostC.address, ':', 'split');
	end
	if(nargin == 0 || isempty(pipename))
		pipename = '/tmp/cl.pipe.ndf.0';
	end
	
	printf('[ndf_monitor] Startup:\n');
	printf('  Input pipe:           %s\n', pipename);
	printf('  Event socket (TiD):   %s\n', hostD.address);
	printf('  Output socket (TiC):  %s\n', hostC.address);
	
	% Event socket
	% - Create a new TCP socket for sending/receiving events
	disp('[ndf_monitor] Creating TiD TDP socket');
	hostD.socket = tr_new();
	tr_init_socket_default(hostD.socket);
	tr_tcpclient(hostD.socket);
	tr_open(hostD.socket);

	% Output socket
	% - Create a new TCP socket for sending probabilities
	disp('[ndf_monitor] Creating TiC TCP socket');
	hostC.socket = tr_new();
	tr_init_socket_default(hostC.socket);
	tr_tcpclient(hostC.socket);
	tr_open(hostC.socket);

	% Pipe opening and NDF configurationf
	% - Here the pipe is opened
	% - ... and the NDF ACK frame is received
	frame = ndf_frame();
	sink = ndf_sink(pipename);
	disp('[ndf_monitor] Receiving ACK...');
	[config, rsize] = ndf_ack(sink);
	
	% NDF ACK check
	% - The NDF id describes the acquisition module running
	% - Bind your modules to a particular configuration (if needed)
	disp(['[ndf_monitor] NDF type id: ' num2str(config.id)]);
	psd.freqs = [2:2:config.sf/4];
	psd.frame = nan(config.sf, config.eeg_channels, length(psd.freqs));

	% Ring-buffers
	% - By default they contain the last half a second of data
	% - Eventually, save CPU by not buffering unuseful data
	disp('[ndf_monitor] Creating ring-buffers');
	buffer.eeg = ndf_ringbuffer(config.sf, config.eeg_channels);
	buffer.exg = ndf_ringbuffer(config.sf, config.exg_channels);
	buffer.tri = ndf_ringbuffer(config.sf, config.tri_channels);
	buffer.tim = ndf_ringbuffer(config.sf/config.samples*10, 1);

	% Initialize ndf_jump structure
	% - Each NDF frame carries an index number
	% - ndf_jump*.m are methods to verify whether your script is
	%   running too slow
	jump = ndf_jump();
	disp('[ndf_monitor] Receiving NDF frames...');
	tic;
	samples = 0;
	loop.tic = ndf_tic();
	while(true) 
		% Read NDF frame from pipe
		loop.toc = ndf_toc(loop.tic);
		loop.tic = ndf_tic();
		[frame, rsize] = ndf_read(sink, config, frame);

		% Acquisition is down, exit
		if(rsize == 0)
			disp('[ndf_monitor] Broken pipe');
			break;
		end
		
		% Buffer NDF streams to the ring-buffers
		buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(frame.timestamp));
		buffer.eeg = ndf_add2buffer(buffer.eeg, frame.eeg);
		buffer.exg = ndf_add2buffer(buffer.exg, frame.exg);
		buffer.tri = ndf_add2buffer(buffer.tri, frame.tri);
		samples = samples + 1;

		% Connect and send data to endpoint
		% - The idea is that the endpoint might go up/down while this module
		%   follows the acquisition
		% - So, if the endpoint falls, we disconnect the socket and wait for the
		%   endpoint to come up again
		% - If the endpoint is connected, send the current TiC/TiD message
		if(tr_connected(hostD.socket) < 0) 
			tr_close(hostD.socket);
			tr_connect(hostD.socket, hostD.ipport{1}, hostD.ipport{2});
			tr_set_nonblocking(hostD.socket, 1);
		else
			if(mod(frame.index, 160) == 0)
				% Prepare TiD message and send it
				idmessage_absolutetic(hostD.message);
				idmessage_setbidx(hostD.message, frame.index);
				idmessage_setevent(hostD.message, frame.index);
				hostD.cache = idmessage_serialize(hostD.serializer);
				tr_send(hostD.socket, hostD.cache);
			end

			% Receive from iD
			if(tr_recv(hostD.socket))
				hostD.buffer = [hostD.buffer, tr_getbuffer(hostD.socket)];
			end
			
			% Dequeue all messages
			[hostD.queue, hostD.buffer] = ...
				ndf_streamerq(hostD.buffer, hostD.hdr, hostD.trl);
			for iq = 1:length(hostD.queue)
				idmessage_deserialize(hostD.serializer, hostD.queue{iq});
			end
		end
		% Same as before, for TiC
		if(tr_connected(hostC.socket) < 0) 
			tr_close(hostC.socket);
			tr_connect(hostC.socket, hostC.ipport{1}, hostC.ipport{2});
		else
			% Prepare TiC message and send it
			icmessage_absolutetic(hostC.message);
			icmessage_setbidx(hostC.message, frame.index);
			icmessage_setvalue(hostC.message, ...
				'ndf_monitor', 'frame', frame.index);
			hostC.cache = icmessage_serialize(hostC.serializer);
			tr_send(hostC.socket, hostC.cache);
		end
			

		% Check if module is running slow
		jump = ndf_jump_update(jump, frame.index);
		if(jump.isjumping)
			disp('[ndf_monitor] Error: running slow');
			break;
		end
	end
	disp(['[ndf_monitor] Received ' ...
		num2str(samples) ' NDF frames in ' num2str(toc) 's']);
catch exception
	disp(['[ndf_monitor] Exception: ' exception.message ]);
	disp(exception.stack);
	disp('[ndf_monitor] Killing Matlab...');
	exit;
end

disp('[ndf_monitor] Closing MEX stuff');
ndf_close(sink);
tr_close(hostC.socket);
tr_free(hostC.socket);
tr_delete(hostC.socket);
