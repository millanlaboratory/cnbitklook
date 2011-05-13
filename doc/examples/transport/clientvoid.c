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
#include <libtransport/tr_net.h>
#include <libtransport/tr_tcp.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define EXAMPLE_NAME "[clientvoid]"

int main(int argc, char * const argv[]) {
	if(argc < 3) {
		printf("Usage: %s ADDRESS PORT\n", EXAMPLE_NAME);
		return 2;
	}

	int i = 0;
	int bytes_send = 0;

	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket(&socket, 1024, 1);
	tr_tcpclient(&socket);
	
	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);

	printf("%s Connecting socket\n", EXAMPLE_NAME);
	while(1) {
		int status = tr_connect(&socket, argv[1], argv[2]);
		if(status == 0)
			break;
		sleep(1);
	}
	
	printf("%s Local TCP socket:  %s:%u [client]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);
	printf("%s Remote TCP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.remote.address, socket.remote.port);
	
	int brecv = 0;
	size_t srecv;
	for(i = 1; i < 11; i++) {
		bytes_send = tr_sendb(&socket, &i, sizeof(int));
		//printf("%s Sending: %d bytes\n", EXAMPLE_NAME, bytes_send);
		
		if(bytes_send == -1) {
			printf("%s Server dropped the connection [send]\n", EXAMPLE_NAME);
			break;
		}
		
		srecv = tr_recv(&socket);
		if(srecv == -1) {
			printf("%s Server dropped the connection [recv]\n", EXAMPLE_NAME);
			break;
		}
		if(srecv > sizeof(int)) {
			printf("Buffer too big!\n");
		} else {
			memcpy(&brecv, socket.buffer, sizeof(int));
			printf("%s Received: %lu %d\n", EXAMPLE_NAME, srecv, brecv);
		}
		
		sleep(0.40);
	}

	printf("%s Closing socket\n", EXAMPLE_NAME);
	tr_close(&socket);

	return 0;
}
