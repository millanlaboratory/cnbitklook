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
%   function [cl, addressD, addressC] = ndf_cl(pipename, addressD, addressC)
function [cl, pipename, addressD, addressC] = ndf_cl(pipename, addressD, addressC)

cl = cl_new();
	
% Connect to the CNBI Loop (CL) infrastructure
if(cl_connect(cl) == false)
	disp('[ndf_cl] Cannot connect to CNBI Loop, killing matlab');
	ndf_cl_close(cl);
	exit;
end

% See weather addressD and addressC are valid port names (i.e. /PORT),
% otherwise assume they are IP:PORT addresses.
if(cl_checkname(pipename) == true)
	pipename = cl_query(cl, pipename);
end
if(cl_checkname(addressD) == true)
	addressD = cl_query(cl, addressD);
end
if(cl_checkname(addressC) == true)
	addressC = cl_query(cl, addressC);
end
