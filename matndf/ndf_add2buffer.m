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

function buffer = ndf_add2buffer(buffer, frame)

[Sb, Cb] = size(buffer);
[Sf, Cf] = size(frame);

if(Cf == 0 || Cb == 0)
    return;
end

if(Cb ~= Cf)
	disp('[ndf_add2buffer] Error: channel size does not match');
	return;
end

if(Sf > Sb)
	disp('[ndf_add2buffer] Error: frame larger than buffer');
	return;
end

buffer = [buffer(Sf+1:end, :); frame];
