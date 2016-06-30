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
%
% See weather loop.cfg.ndf.id and loop.cfg.ndf.ic are valid port names (i.e.
% /PORT), otherwise die

function loop  = ndf_loopnames(loop)

if(isempty(loop.cfg.ndf.pipe) == false)
	if(cl_checkname(loop.cfg.ndf.pipe) == true)
		loop.cfg.ndf.pipe = cl_query(loop.cl, loop.cfg.ndf.pipe);
		if(isempty(loop.cfg.ndf.pipe))
			disp(['[ndf_checknames] Warning: pipename not found!']);
		end
	else
		disp('[ndf_checknames] Warning: pipename in wrong format');
	end
else
	disp('[ndf_checknames] Pipename is empty, XML configuration required');
end
