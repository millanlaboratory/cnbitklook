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

#include "generator.h"
#include <unistd.h>

int main(void) {
	int NUMCLASS = 6;
	int idx_selected_class = 5;
	int i;
	float recv_probs[NUMCLASS];

	for (i = 0; i < 10; i++) {
		co_generator_create(recv_probs, NUMCLASS, idx_selected_class);	
		co_generator_print(recv_probs, NUMCLASS);	
		sleep(1);
	}

	return 0;
}
