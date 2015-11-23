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

if(ndf.sink); 
	ndf_close(ndf.sink); 
end

if(loop.cfg.config)
	loop.cfg.config = ccfg_delete(loop.cfg.config); 
end

if(loop.cfg.taskset);
	loop.cfg.taskset = ccfgtaskset_delete(loop.cfg.taskset);
end
	
if(loop.sC); 
	loop.sC = icserializerrapid_delete(loop.sC); 
end

if(loop.sDi); 
	loop.sDi = idserializerrapid_delete(loop.sDi); 
end

if(loop.sDo); 
	loop.sDo = idserializerrapid_delete(loop.sDo); 
end

if(loop.mC); 
	loop.mC = icmessage_delete(loop.mC); 
end

if(loop.mDi); 
	loop.mDi = idmessage_delete(loop.mDi); 
end

if(loop.mDo); 
	loop.mDo = idmessage_delete(loop.mDo); 
end

if(loop.iC)
	tic_detach(loop.iC);
	loop.iC = tic_delete(loop.iC);
end
fclose('all');
