/*
    Copyright (C) 2009 Luca Tonin <luca.tonin@epfl.ch>
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiprotocol library

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

#ifndef FPGENERATOR_H
#define FPGENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void co_generator_print(float* probs_array, int num_classes); 
void co_generator_create (float* probs_array, int num_classes, int selected_class);
float co_generator_rand(float min, float max);

#ifdef __cplusplus
}
#endif

#endif
