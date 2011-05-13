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
function [message, buffer] = ndf_streamer(buffer, hdr, trl)

message = '';

pos.hdr = strfind(buffer, hdr);
pos.trl = strfind(buffer, trl);

if(isempty(pos.hdr) | isempty(pos.trl))
	return;
end

pos.N = min([length(pos.hdr) length(pos.trl)]);

pos.cbeg = pos.hdr(1);
pos.cend = pos.trl(1) + length(trl) - 1;
pos.ccut = pos.trl(1) + length(trl);

message = buffer(pos.cbeg:pos.cend);
buffer = buffer(pos.ccut:end);
