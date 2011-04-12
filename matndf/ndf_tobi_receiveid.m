%   Copyright (C) 2011 Michele Tavella <tavella.michele@gmail.com>
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
%   function hasmessages = ndf_tobi_receiveid(tobi)
function messages = ndf_tobi_receiveid(tobi)

% Receive from iD
if(tr_recv(tobi.iD.socket))
	tobi.iD.buffer = [tobi.iD.buffer, tr_getbuffer(tobi.iD.socket)];
end

% Dequeue all messages
tobi.iD.queue = {};
[tobi.iD.queue, tobi.iD.buffer] = ...
	ndf_streamerq(tobi.iD.buffer, tobi.iD.hdr, tobi.iD.trl);

messages = length(tobi.iD.queue);
