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

function [messages, buffer] = ndf_streamerq(buffer, hdr, trl)

messages = {};

cache = '';
while(true) 
	[cache, buffer] = ndf_streamer(buffer, hdr, trl);
	if(isempty(cache) == false)
		messages{end+1} = cache;
	else
		break
	end
end
