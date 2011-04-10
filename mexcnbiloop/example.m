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

clear all;

loop = cl_new();
if(cl_connect(loop) == false)
	disp('[example] Cannot connect...');
end

for i = 1:10
	if(cl_isconnected(loop) == false)
		disp('[example] Lost connection...');
		break;
	end
	disp('[example] Still connected!');

	pause(1);
end

cl_disconnect(loop);

cl_delete(loop);
