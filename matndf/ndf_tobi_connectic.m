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
function isconnected = ndf_tobi_connectic(tobi, cl, addressC);


if(tr_connected(tobi.iC.socket) < 0) 
	isconnected = false;
	tr_close(tobi.iC.socket);
	
	% This is meant to be used with extreme care because 
	% it introduces a delay in the loop
	if(nargin == 3)
		addressC = cl_query(cl, addressC);
		if(isempty(addressC) == false)
			tobi.iC.address = addressC;
			tobi.iC.ipport 	= regexp(tobi.iC.address, ':', 'split');
		end
	end
	if(length(tobi.iC.ipport) == 2)
		tr_connect(tobi.iC.socket, tobi.iC.ipport{1}, tobi.iC.ipport{2});
	end
else
	isconnected = true;
end
