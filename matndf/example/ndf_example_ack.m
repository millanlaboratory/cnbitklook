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
%   function config = ndf_example_ack(pipename, hostname, port, doplot, dopsd)
function config = ndf_example_ack(pipename, hostname, port, doplot, dopsd)

try 
	if(nargin < 5 || isempty(dopsd))
		dopsd = false;
	end
	if(nargin < 4 || isempty(doplot))
		doplot = false;
	end
	if(nargin < 3 || isempty(port))
		port = '9000';
	end
	if(nargin < 2 || isempty(hostname))
		hostname = '127.0.0.1';
	end
	if(nargin == 0 || isempty(pipename))
		pipename = '/tmp/ndf.example.ack';
	end
	
	printf('[ndf_example_ack] Startup:\n');
	printf('  Input pipe:      %s\n',    pipename);
	printf('  Output socket:   %s:%s\n', hostname, port);
	printf('  Plot/PDF:        %d/%d\n', doplot, dopsd);

	% Configuration 
	% - Initialize, configure and setup your stuff in here
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

	% Output socket
	% - Create a new TCP socket for sending probabilities
	disp('[ndf_example_ack] Creating TCP socket');
	socket = tr_new();
	tr_init_socket_default(socket);
	tr_tcpclient(socket);
	tr_open(socket);

	% Pipe opening and NDF configurationf
	% - Here the pipe is opened
	% - ... and the NDF ACK frame is received
	frame = ndf_frame();
	sink = ndf_sink(pipename);
	disp('[ndf_example_ack] Receiving ACK...');
	[config, rsize] = ndf_ack(sink);
	
	% NDF ACK check
	% - The NDF id describes the acquisition module running
	% - Bind your modules to a particular configuration (if needed)
	disp(['[ndf_example_ack] NDF type id: ' num2str(config.id)]);
	psd.freqs = [2:2:config.sf/4];
	psd.frame = nan(config.sf, config.eeg_channels, length(psd.freqs));

	% Ring-buffers
	% - By default they contain the last half a second of data
	% - Eventually, save CPU by not buffering unuseful data
	disp('[ndf_example_ack] Creating ring-buffers');
	buffer.eeg = ndf_ringbuffer(config.sf, config.eeg_channels);
	buffer.exg = ndf_ringbuffer(config.sf, config.exg_channels);
	buffer.tri = ndf_ringbuffer(config.sf, config.tri_channels);
	buffer.tim = ndf_ringbuffer(config.sf/config.samples*10, 1);

	% Initialize ndf_jump structure
	% - Each NDF frame carries an index number
	% - ndf_jump*.m are methods to verify whether your script is
	%   running too slow
	jump = ndf_jump();
	disp('[ndf_example_ack] Receiving NDF frames...');
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
			disp('[ndf_example_ack] Broken pipe');
			break;
		end
		
		% Buffer NDF streams to the ring-buffers
		buffer.tim = ndf_add2buffer(buffer.tim, ndf_toc(frame.timestamp));
		buffer.eeg = ndf_add2buffer(buffer.eeg, frame.eeg);
		buffer.exg = ndf_add2buffer(buffer.exg, frame.exg);
		buffer.tri = ndf_add2buffer(buffer.tri, frame.tri);
		samples = samples + 1;

		% Compute PSDs if acquisition is gtec 
		if(dopsd)
			for ch = 1:config.eeg_channels
				eegc3_psd(buffer.eeg(:, ch), psd.freqs, config.sf, 0.5, 0.5);
			end
		end

		% Connect and send data to endpoint
		% - The idea is that the endpoint might go up/down while this module
		%   follows the acquisition
		% - So, if the endpoint falls, we disconnect the socket and wait for the
		%   endpoint to come up again
		% - If the endpoint is connected, send the current frame number
		if(tr_connected(socket) < 0) 
			tr_close(socket);
			tr_connect(socket, hostname, port);
		else
			tr_send(socket, num2str(frame.index));
		end

		% Plot received data and acquisition delays 
		% - Plotting is just useful for fun or debugging
		% - Matlab uses a lot of resources for this
		% - Never plot when in production
		if(doplot)
			eegc3_figure(1);
			subplot(8, 1, 1:4)
				imagesc(eegc3_car(eegc3_dc(buffer.eeg))');
				title(sprintf('Frame=%6.6d, Dt=%7.2f (ms), Dl=%7.2f (ms)', ...
					frame.index, buffer.tim(end), loop.toc));
				ylabel('eeg');
			subplot(8, 1, 5:6)
				imagesc(eegc3_car(eegc3_dc(buffer.exg))');
				ylabel('exg');
			subplot(8, 1, 7)
				imagesc(buffer.tri');
				ylabel('tri');
			subplot(8, 1, 8)
				imagesc(buffer.tim', [0 250]);
				ylabel('tim');
			drawnow;
		end

		% Check if module is running slow
		jump = ndf_jump_update(jump, frame.index);
		if(jump.isjumping)
			disp('[ndf_example_ack] Error: running slow');
			break;
		end
	end
	disp(['[ndf_example_ack] Received ' ...
		num2str(samples) ' NDF frames in ' num2str(toc) 's']);
catch exception
	disp(['[ndf_example_ack] Exception: ' exception.message ]);
	disp(exception.stack);
end

disp('[ndf_example_ack] Closing MEX stuff');
ndf_close(sink);
tr_close(socket);
tr_free(socket);
tr_delete(socket);

%exit;
