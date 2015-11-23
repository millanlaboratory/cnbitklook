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
function fid = ndf_sink(filename, waits)

fid = -1;

if(nargin == 1)
	waits = Inf;
end

disp(['[ndf_sink] Opening pipe: ' filename ' (' num2str(waits) ' seconds)']);

if(waits <= 0)
	fid = fopen(filename, 'r', 'ieee-le');
	if(fid <= 0)
		disp('[ndf_sink] Error: cannot open pipe');
	end
	disp(['[ndf_sink] Sink pipe opened: ' num2str(fid)]);
	return;
end

tic;
while(toc < waits)
	fid = fopen(filename, 'r', 'ieee-le');
	if(fid >= 0)
		disp(['[ndf_sink] Sink pipe opened: ' num2str(fid)]);
		return;
	end
	pause(1);
end
disp('[ndf_sink] Error: cannot open pipe, even by waiting');
