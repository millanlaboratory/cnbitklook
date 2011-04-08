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

function frame = ndf_frame(frame)

if(nargin == 0)
	frame.timestamp = [];
	frame.index     = [];
	frame.labels    = [];
	frame.eeg		= [];
	frame.exg		= [];
	frame.tri		= [];
else
	frame.timestamp = NaN * frame.timestamp;
	frame.index     = NaN * frame.index;
	frame.labels    = NaN * frame.labels;
	frame.eeg		= NaN * frame.eeg;
	frame.exg		= NaN * frame.exg;
	frame.tri		= NaN * frame.tri;
	frame.beeg		= NaN * frame.beeg;
	frame.bexg		= NaN * frame.bexg;
	frame.btri		= NaN * frame.btri;
end

frame.size.timestamp = NaN;
frame.size.index     = NaN;
frame.size.labels    = NaN;
frame.size.beeg		= NaN;
frame.size.bexg		= NaN;
frame.size.btri		= NaN;
