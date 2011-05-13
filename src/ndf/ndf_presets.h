/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef NDFPRESETS_H
#define NDFPRESETS_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "ndf_codec.h"

#define NDF_ID_TINY 				1000
#define NDF_ID_HUGE 				1001
#define NDF_ID_GTEC_016_0512		2000
#define NDF_ID_GTEC_016_2048		2001
#define NDF_ID_BIOSEMI_032_2048		3000
#define NDF_ID_BIOSEMI_064_2048		3001
#define NDF_ID_BIOSEMI_128_2048		3002
#define NDF_ID_BIOSEMI_256_2048		3003

void ndf_preset_tiny(ndf_frame* frame);
void ndf_preset_huge(ndf_frame* frame);
void ndf_preset_gtec_016_0512(ndf_frame* frame);
void ndf_preset_gtec_016_2048(ndf_frame* frame);
void ndf_preset_biosemi_032_2048(ndf_frame* frame);
void ndf_preset_biosemi_064_2048(ndf_frame* frame);
void ndf_preset_biosemi_128_2048(ndf_frame* frame);
void ndf_preset_biosemi_256_2048(ndf_frame* frame);

#ifdef __cplusplus
}
#endif

#endif
