%   Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>
%
%   This file is part of the mexcnbiconfig wrapper
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

function example_record()

loop = cl_new();
if(cl_connect(loop) == false)
	disp('[example_xdf] Cannot connect...');
else
	timestamp = datestr(now, 'yyyy.mm.dd-HH:MM:SS');
	filexdf = ['recording_' timestamp '.bdf'];
	filelog = 'recording.log';
	linelog = ['Simple recording at ' date() ];
	
	input('[example_xdf] Press ENTER to start recording');
	cl_openxdf(loop, filexdf, filelog, linelog);
	
	input('[example_xdf] Press ENTER to stop recording');
	cl_closexdf(loop);
end
cl_disconnect(loop);
cl_delete(loop);
