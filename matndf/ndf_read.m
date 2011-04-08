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

function [frame, count] = ndf_read(fid, config, frame)

[frame.timestamp, frame.size.timestamp] = ...
	fread(fid, config.tim_size, config.tim_type, 'ieee-le');
[frame.index, frame.size.index] = ... 
	fread(fid, config.idx_size, config.idx_type, 'ieee-le');
[frame.labels, frame.size.labels] = ... 
	fread(fid, config.labels, config.lbl_type, 'ieee-le');
[frame.beeg, frame.size.beeg] = ... 
	fread(fid, config.samples*config.eeg_channels, config.eeg_type, 'ieee-le');
[frame.bexg, frame.size.bexg] = ... 
	fread(fid, config.samples*config.exg_channels, config.exg_type, 'ieee-le');
[frame.btri, frame.size.btri] = ... 
	fread(fid, config.samples*config.tri_channels, config.tri_type, 'ieee-le');

count = frame.size.timestamp + frame.size.index + ...
	frame.size.labels + ...
	frame.size.beeg + frame.size.bexg + frame.size.btri;

if(count ~= config.rcount)
	if(count > 0)
		disp(['[ndf_read] Error: NDF frame does not match expected size: ' ... 
			num2str(count) '/' num2str(config.rcount)]);
	else
		disp('[ndf_read] Error: broken pipe');
	end
	frame = ndf_frame(frame);
	count = 0;
	return;
end

frame.eeg = reshape(frame.beeg, config.eeg_channels, config.samples)';
frame.exg = reshape(frame.bexg, config.exg_channels, config.samples)';
frame.tri = reshape(frame.btri, config.tri_channels, config.samples)';
