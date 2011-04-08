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

int main(void) {
	unsigned int occ[3] = {5, 5, 5};

	printf("\nUsing irand and randomize\n");
	printf("First run:    ");
	co_stimula_sequence seq = co_stimula_init(3, occ, NULL);
	co_stimula_randomize(&seq);
	co_stimula_print(&seq);

	printf("Second run:   ");
	co_stimula_sequence seq_b = co_stimula_init(3, occ, NULL);
	co_stimula_randomize(&seq_b);
	co_stimula_print(&seq_b);

	printf("\nChecking rrand and randstim...\n");
	co_stimula_seedrand ();
	printf("First run:    ");
	co_stimula_sequence seq_r = co_stimula_init(3, occ, NULL);
	co_stimula_randstim(&seq_r);
	co_stimula_print(&seq_r);
	printf("Second run:   ");
	co_stimula_sequence seq_s = co_stimula_init(3, occ, NULL);
	co_stimula_randstim(&seq_s);
	co_stimula_print(&seq_s);

	return 0;
}
