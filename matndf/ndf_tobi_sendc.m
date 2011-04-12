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
%   function bytes = ndf_tobi_sendc(tobi, frameidx)
function bytes = ndf_tobi_sendc(tobi, frameidx)

icmessage_absolutetic(tobi.iC.message);
icmessage_setbidx(tobi.iC.message, frameidx);
tobi.iC.cache = icmessage_serialize(tobi.iC.serializer);
bytes = tr_send(tobi.iC.socket, tobi.iC.cache);
