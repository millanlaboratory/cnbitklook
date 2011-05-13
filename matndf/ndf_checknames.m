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
function [pipename, addressD, addressC] = ndf_checknames(cl, pipename, addressD, addressC)

% See weather addressD and addressC are valid port names (i.e. /PORT),
% otherwise die
if(isempty(pipename) == false)
	if(cl_checkname(pipename) == true)
		pipename = cl_query(cl, pipename);
		if(isempty(pipename))
			disp(['[ndf_checknames] Warning: pipename not found!']);
		end
	else
		disp('[ndf_checknames] Warning: pipename in wrong format');
	end
else
	disp('[ndf_checknames] Pipename is empty, XML configuration required');
end

if(isempty(addressD) == false)
	if(cl_checkname(addressD) == true)
		addressD = cl_query(cl, addressD);
		if(isempty(addressD))
			disp(['[ndf_checknames] Warning: iD address not found!']);
		end
	else
		disp('[ndf_checknames] Warning: iD address in wrong format');
	end
else
	disp('[ndf_checknames] iD address is empty, XML configuration required');
end

if(isempty(addressC) == false)
	if(cl_checkname(addressC) == true)
		addressC = cl_query(cl, addressC);
		if(isempty(addressC))
			disp(['[ndf_checknames] Warning: iC address not found!']);
		end
	else
		disp('[ndf_checknames] Warning: iC address in wrong format');
	end
else
	disp('[ndf_checknames] iC address is empty, XML configuration required');
end
