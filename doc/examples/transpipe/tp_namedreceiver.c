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
#include <libtranspipe/tp_namedpipe.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
	int i = 0;
	float vector[4]; 
	size_t bsize = 4*sizeof(float);
	size_t wsize = 0;

	tp_npipe pipe;
	tp_init(&pipe, "/tmp/tpnpipe0");

	if(tp_openread(&pipe) <= 0) {
		printf("Cannot open pipe for reading\n");
		exit(1);
	}
	
	while(1) {
		wsize = tp_read(&pipe, (char*)vector, bsize);
		if(wsize <= 0) {
			printf("Cannot read pipe\n");
			break;
		}
		printf("Read from pipe: ");
		for(i = 0; i < 4; i++) 
			printf("%f ", vector[i]);
		printf("\n");
	}

	if(tp_close(&pipe) < 0) {
		printf("Cannot close pipe\n");
		exit(1);
	}
	
	printf("Going down\n");
	return 1;
}
