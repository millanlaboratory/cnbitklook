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

function config = ndf_config()

config.buffer 		= [];
config.id 			= 0;
config.sf 			= 0;
config.labels 		= 0;
config.samples 		= 0;
config.eeg_channels = 0;
config.exg_channels = 0;
config.tri_channels = 0;
config.tim_type 	= 'unset';
config.eeg_type 	= 'unset';
config.exg_type 	= 'unset';
config.tri_type 	= 'unset';
config.lbl_type 	= 'unset';
% Extra stuff I need in ndf_ack.m: maybe I will enclose it with the stuff below
% in a sub-structure. Maybe. Or maybe you can branch me, fix, test and
% send a patch to Michele for Xmas.
config.tsize 		= 0;
config.rcount 		= 0;
% Frame index is always transmitted as uint64
config.idx_type 	= 'uint64';
config.idx_size 	= 1;
