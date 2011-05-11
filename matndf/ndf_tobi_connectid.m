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
%	function status = ndf_tobi_connectid(tobi);
function isconnected = ndf_tobi_connectid(tobi, cl, addressD);


if(tr_connected(tobi.iD.socket) < 0) 
	isconnected = false;
	tr_close(tobi.iD.socket);

	% This is meant to be used with extreme care because 
	% it introduces a delay in the loop
	if(nargin == 3)
		addressD = cl_query(cl, addressD);
		if(isempty(addressD) == false)
			tobi.iD.address = addressD;
			tobi.iD.ipport 	= regexp(tobi.iD.address, ':', 'split');
		end
	end
	if(length(tobi.iD.ipport) == 2)
		tr_connect(tobi.iD.socket, tobi.iD.ipport{1}, tobi.iD.ipport{2});
		tr_set_nonblocking(tobi.iD.socket, 1);
	end
else
	isconnected = true;
end
