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

function [config, count] = ndf_ack(fid)

config = ndf_config();

[config.buffer, count] = fread(fid, 12, 'uint', 'ieee-le');

if(count ~= 12)
	if(count > 0)
		disp(['[ndf_ack] Error: NDF ACK does not match expected size: ' ... 
			num2str(count) '/12']);
	else
		disp('[ndf_ack] Error: broken pipe');
	end
	count = 0;
	return;
end

types = ndf_types();
config.id 			= config.buffer(1);
config.sf 			= config.buffer(2);
config.labels 		= config.buffer(3);
config.samples 		= config.buffer(4);
config.eeg_channels = config.buffer(5);
config.exg_channels = config.buffer(6);
config.tri_channels = config.buffer(7);
try 
	config.eeg_type 	= types{config.buffer(9)};
	config.exg_type 	= types{config.buffer(10)};
	config.tri_type 	= types{config.buffer(11)};
	config.lbl_type 	= types{config.buffer(12)};
catch execption
	disp('[ndf_ack] Fatal: NDF ACK corrupted.');
	disp('          Probably two processes are reading from the same pipe.');
	disp('          What happened is so bad that I will die on the spot.');
	disp('[ndf_ack] Killing matlab');
	exit;
end

% Check if timestamp is sent as a struct timeval
if(config.buffer(8) == 11)
		disp('[ndf_ack] Warning: NDF timestamp in compatibility mode');
	config.tim_type = 'uchar';
	config.tim_size = 16;
else
	config.tim_type = types{config.buffer(8)};
	config.tim_size = 1;
end

% Expected number of "numbers" to read from the pipe
% Once again, Matlab sucks.
config.rcount = config.tim_size + config.idx_size + ...
	config.eeg_channels * config.samples + ...
	config.exg_channels * config.samples + ...
	config.tri_channels * config.samples + ...
	config.labels;
