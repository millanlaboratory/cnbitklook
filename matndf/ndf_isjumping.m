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

function [jump] = ndf_jump_update(jump, index)

if(jump.old == -1)
	jump.isjumping = false;
	if(index <= 0)
		return;
	end
	jump.old = index;
	jump.now = index;
	return;
end
jump.old = jump.now;
jump.now = index;
jump.diff = jump.now - jump.old;
jump.isjumping = jump.diff > 1;
