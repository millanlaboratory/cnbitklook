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
%   function bytes = ndf_tobi_sendd(tobi, frameidx)
function bytes = ndf_tobi_sendd(tobi, frameidx)

idmessage_absolutetic(tobi.iD.message);
idmessage_setbidx(tobi.iD.message, frameidx);
tobi.iD.cache = idmessage_serialize(tobi.iD.serializer);
bytes = tr_send(tobi.iD.socket, tobi.iD.cache);
