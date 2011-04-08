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

#include "stimula.h"

co_stimula_sequence co_stimula_init_old(unsigned int num_class, 
		unsigned int num_stim, unsigned int offset) {
	printf("co_stimula_init_old is deprecated! Stop using it!\n");
	assert(num_stim < EXPERIMENT_MAXSTIM);
	co_stimula_sequence stim;
	memset(&stim, 0, sizeof(stim));

	int c_stim, c_class;
	int stim4class;

	stim.num_class = num_class;
	stim.num_stim = num_stim;
	
	stim4class = num_stim/num_class;	
	for (c_class = 0; c_class < num_class; c_class++) 
		for(c_stim = c_class*stim4class; c_stim < (c_class + 1)*stim4class; 
				c_stim++) 
			stim.sequence[c_stim] = c_class + offset;

	return stim;
}

co_stimula_sequence co_stimula_init(unsigned int classes, 
		unsigned int* occurrence, unsigned int* classes_labels) {
	unsigned int k = 0, o = 0, s = 0;

	assert(occurrence != NULL);


	co_stimula_sequence stim;
	memset(&stim, 0, sizeof(stim));

	stim.num_class = classes;
	stim.num_stim = 0;

	for(k = 0; k < classes; k++) {
		stim.num_stim += occurrence[k];
		for(o = 0; o < occurrence[k]; o++)  
			if (classes_labels == NULL) {
				stim.sequence[s++] = k;
			} else {
				stim.sequence[s++] = classes_labels[k];
			}
	}

	return stim;
}

unsigned int co_stimula_irand(unsigned int limit) {
	srand(time(NULL));
	return (unsigned int)(rand() % (int)limit);
}

unsigned int co_stimula_rrand (unsigned int lower, unsigned int upper) {
	float range = (float)upper-lower+1;
	// 'rand' to be initialised somewhere else, see co_stimula_seedrand
	return lower+(unsigned int)(range*rand()/(RAND_MAX + 1.0));
}

void co_stimula_seedrand (void){
	srand(time(NULL));
}

void co_stimula_randomize(co_stimula_sequence* stim) {
	unsigned int n_stim;
	unsigned int curr_pos;
	unsigned int new_pos;
	unsigned int curr_val;
	unsigned int new_val;

	n_stim = stim->num_stim;
	for(curr_pos = 0; curr_pos < n_stim; curr_pos++) {
		new_pos = (curr_pos) + co_stimula_irand(n_stim - curr_pos);	
		
		curr_val = stim->sequence[curr_pos];
		new_val = stim->sequence[new_pos];
		stim->sequence[curr_pos] = new_val;
		stim->sequence[new_pos] = curr_val;
	}
}

void co_stimula_randstim(co_stimula_sequence* stim) {
	unsigned int n_stim;
	unsigned int curr_pos;
	unsigned int new_pos;
	unsigned int curr_val;
	unsigned int new_val;

	n_stim = stim->num_stim;
	for(curr_pos = 0; curr_pos < n_stim; curr_pos++) {
		new_pos = co_stimula_rrand(curr_pos, n_stim - 1);	
		curr_val = stim->sequence[curr_pos];
		new_val = stim->sequence[new_pos];
		stim->sequence[curr_pos] = new_val;
		stim->sequence[new_pos] = curr_val;
	}
}

void co_stimula_print(co_stimula_sequence* stim) {
	unsigned int i;
	for(i = 0; i < stim->num_stim; i++) 
		printf("%d ", stim -> sequence[i]);
	printf("\n");
}
