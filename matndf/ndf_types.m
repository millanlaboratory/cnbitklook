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

function types = ndf_types()

types = {};
types{1}	= 'uint8';
types{2}	= 'uint16';
types{3}	= 'uint32';
types{4}	= 'uint64';
types{5}	= 'int8';
types{6}	= 'int16';
types{7}	= 'int32';
types{8}	= 'int64';
types{9}	= 'float';
types{10}	= 'double';
types{11}	= 'struct timeval';
types{12}	= 'double';
