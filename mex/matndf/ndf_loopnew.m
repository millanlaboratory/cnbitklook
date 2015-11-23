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

loop.uptime = ndf_tic();
loop.exit = true;
loop.cl   = cl_new();
loop.jump = ndf_jump();
loop.cfg  = ccfg_new();
loop.iC   = tic_newsetonly();
loop.iD   = tid_new();
loop.mC   = icmessage_new();
loop.mDi  = idmessage_new();
loop.mDo  = idmessage_new();
loop.sC   = icserializerrapid_new(loop.mC);
loop.sDi  = idserializerrapid_new(loop.mDi);
loop.sDo  = idserializerrapid_new(loop.mDo);
