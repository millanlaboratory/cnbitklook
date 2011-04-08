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

#ifndef STIMULA_H
#define STIMULA_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EXPERIMENT_MAXSTIM 8192

typedef struct co_stimula_struct {
	unsigned int num_class;
	unsigned int num_stim;
	unsigned int sequence[EXPERIMENT_MAXSTIM];
} co_stimula_sequence;

unsigned int co_stimula_irand(unsigned int limit);
unsigned int co_stimula_rrand (unsigned int lower, unsigned int upper);
void co_stimula_seedrand (void);
co_stimula_sequence co_stimula_init_old(unsigned int num_class, 
		unsigned int num_stim, unsigned int offset);
co_stimula_sequence co_stimula_init(unsigned int classes,
		unsigned int* occurrence, unsigned int* classes_labels);
void co_stimula_randomize(co_stimula_sequence* stim);
void co_stimula_randstim(co_stimula_sequence* stim);
void co_stimula_print(co_stimula_sequence* stim);

#ifdef __cplusplus
}
#endif

#endif
