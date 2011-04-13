/*
	Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libndf library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NDFPRESETS_C
#define NDFPRESETS_C

#include "ndf_presets.h"
#include "ndf_types.h"
#include <stdlib.h>
#include <string.h>

void ndf_preset_tiny(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 4;
	frame->config.eeg_channels 	= 4;
	frame->config.exg_channels 	= 3;
	frame->config.tri_channels 	= 2;
	frame->config.samples 		= 16;
	frame->config.sf 			= 512;
	frame->config.id 			= NDF_ID_TINY;
}

void ndf_preset_huge(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 1024;
	frame->config.eeg_channels 	= 1024;
	frame->config.exg_channels 	= 1024;
	frame->config.tri_channels 	= 1;
	frame->config.samples 		= 4096;
	frame->config.sf 			= 8192;
	frame->config.id 			= NDF_ID_HUGE;
}

void ndf_preset_gtec_016_0512(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 64;
	frame->config.eeg_channels 	= 16;
	frame->config.exg_channels 	= 0;
	frame->config.tri_channels 	= 1;
	frame->config.samples 		= 32;
	frame->config.sf 			= 512;
	frame->config.id 			= NDF_ID_GTEC_016_0512;
}

void ndf_preset_gtec_016_2048(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 64;
	frame->config.eeg_channels 	= 16;
	frame->config.exg_channels 	= 0;
	frame->config.tri_channels 	= 1;
	frame->config.samples 		= 128;
	frame->config.sf 			= 2048;
	frame->config.id 			= NDF_ID_GTEC_016_2048;
}

void ndf_preset_biosemi_032_2048(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 0;
	frame->config.eeg_channels 	= 32;
	frame->config.exg_channels 	= 8;
	frame->config.tri_channels 	= 1;
	frame->config.samples 		= 64;
	frame->config.sf 			= 2048;
	frame->config.id 			= NDF_ID_BIOSEMI_032_2048;
}

void ndf_preset_biosemi_064_2048(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 0;
	frame->config.eeg_channels 	= 64;
	frame->config.exg_channels 	= 8;
	frame->config.tri_channels 	= 1;
	frame->config.samples 		= 64;
	frame->config.sf 			= 2048;
	frame->config.id 			= NDF_ID_BIOSEMI_064_2048;
}

void ndf_preset_biosemi_128_2048(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 0;
	frame->config.eeg_channels 	= 128;
	frame->config.exg_channels 	= 8;
	frame->config.tri_channels 	= 1;
	frame->config.samples 		= 64;
	frame->config.sf 			= 2048;
	frame->config.id 			= NDF_ID_BIOSEMI_128_2048;
}

void ndf_preset_biosemi_256_2048(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= NDF_DOUBLE;
	frame->types.exg 			= NDF_DOUBLE;
	frame->types.tri 			= NDF_INT32;
	frame->config.labels 		= 0;
	frame->config.eeg_channels 	= 256;
	frame->config.exg_channels 	= 8;
	frame->config.tri_channels 	= 1;
	frame->config.samples 		= 64;
	frame->config.sf 			= 2048;
	frame->config.id 			= NDF_ID_BIOSEMI_256_2048;
}
#endif
