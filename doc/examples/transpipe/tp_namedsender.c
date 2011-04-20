/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libtranspipe library

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

#include <libtranspipe/tp_namedpipe.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
	int i = 0;
	int j = 0;
	float vector[4]; 
	size_t bsize = 4*sizeof(float);
	size_t wsize = 0;

	tp_npipe pipe;
	tp_init(&pipe, "/tmp/tpnpipe0");
	if(tp_create(&pipe) < 0)
		printf("Cannot create pipe\n");

	if(tp_openwrite(&pipe) <= 0)
		printf("Cannot open pipe for writing\n");
	tp_catchsigpipe();

	for(i = 0; i < 10; i++) {
		for(j = 0; j < 4; j++) 
			vector[j] = i;
		wsize = tp_write(&pipe, (char*)vector, bsize);
		if(wsize <= 0) {
			printf("Cannot write pipe\n");
			break;
		}
		if(tp_receivedsigpipe() == 1) {
			printf("Broken pipe\n");
			break;
		}

		printf("Written to pipe: %d\n", i);
		sleep(1);
	}
	
	if(tp_close(&pipe) < 0) {
		printf("Cannot close pipe\n");
		exit(1);
	}
	
	if(tp_remove(&pipe) != 0) {
		printf("Cannot remove pipe\n");
		exit(1);
	}
	
	printf("Going down\n");
	return 1;
}
