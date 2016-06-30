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

clear all;
addpath('../');

disp('[ndf_example_gtec] Creating buffer');
buffer = ndf_ringbuffer(2048, 16);

frame = ndf_frame_legacy();
sink = ndf_sink('/tmp/ndf.example.legacy');

samples.data = [];
samples.tot  = 0;

disp('[ndf_example_legacy] Receiving NDF frames...');
tic;
while(true) 
	[frame, rsize] = ndf_read_legacy(sink, frame, 16, 128);

	if(rsize == 0)
		disp('[ndf_example_legacy] Broken pipe');
		break;
	end
	figure(1);
	imagesc(frame.samples');
	drawnow;
	
	buffer = ndf_add2buffer(buffer, frame.samples);
	samples.tot = samples.tot + 1;
end
disp(['[ndf_example_legacy] Received ' ...
	num2str(samples.tot) ' NDF frames in ' num2str(toc) 's']);

ndf_close(sink);
