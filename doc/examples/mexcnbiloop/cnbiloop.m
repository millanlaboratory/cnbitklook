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
else
	cl_checkname('test');
	cl_checkname('/test');

	disp(['[example] Acquisition: ' cl_query(loop, '/acquisition')]);
	disp(['[example] Processing:  ' cl_query(loop, '/processing')]);
	disp(['[example] Nameserver:  ' cl_query(loop, '/nameserver')]);

	cl_openxdf(loop, 'matlab_xdf.bdf', 'matlab_log.txt', 'nothing special');
	for i = 1:10
		if(cl_isconnected(loop) == false)
			disp('[example] Lost connection...');
			break;
		end
		disp('[example] Still connected!');
		
		cl_set(loop, '/test', '1.2.3.4:1000');
		cl_query(loop, '/test');
		cl_unset(loop, '/test');
		cl_store(loop, 'data0', '<hello>world!</hello>');
		cl_retrieve(loop, 'data0');
		cl_erase(loop, 'data0');
		cl_storefile(loop, 'huge', '/home/mtavella/Desktop/huge.txt')
		cl_retrievefile(loop, 'huge', '/home/mtavella/Desktop/huge.txt');
		cl_erase(loop, 'huge');
		pause(1);
	end
	cl_closexdf(loop);
end
cl_disconnect(loop);
cl_delete(loop);
