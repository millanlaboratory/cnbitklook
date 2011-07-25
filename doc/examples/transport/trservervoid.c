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

#include <transport/tr_net.h>
#include <transport/tr_tcp.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define EXAMPLE_NAME "[servervoid]"

int main(int argc, char * const argv[]) {
	if(argc < 3) {
		printf("Usage: %s IP PORT\n", EXAMPLE_NAME);
		return 2;
	}

	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket(&socket, 1024, 1);
	tr_tcpserver(&socket);

	tr_socket endpoint;
	tr_init_socket(&endpoint, 1024, 1);
	tr_tcpendpoint(&endpoint);
	
	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);

	printf("%s Binding socket\n", EXAMPLE_NAME);
	tr_bind(&socket, argv[1], argv[2]);
	
	printf("%s Local TCP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);

	printf("%s Listening\n", EXAMPLE_NAME);
	tr_listen(&socket);
	
	size_t srecv;
	int bsend = 666, brecv = 0;
	printf("%s Entering accept loop\n", EXAMPLE_NAME);
	while(1) {
		printf("%s Waiting for endpoint\n", EXAMPLE_NAME);
		tr_accept(&socket, &endpoint);
	
		printf("%s Remote TCP socket: %s:%u [endpoint]\n", EXAMPLE_NAME, 
				endpoint.remote.address, endpoint.remote.port);

		while(1) {
			srecv = tr_recv(&endpoint);
			if(srecv <= 0)
				break;
			if(srecv > sizeof(int)) {
				printf("Buffer too big!\n");
			} else {
				memcpy(&brecv, endpoint.buffer, sizeof(int));
				printf("%s Received: %d\n", EXAMPLE_NAME, brecv);
			}
			bsend = brecv + 10;
			if(tr_sendb(&endpoint, &bsend, sizeof(int)) == -1)
				break;
		}
		printf("%s Endpoint dropped the connection\n", EXAMPLE_NAME);
		tr_close(&endpoint);
	}

	printf("%s Closing socket\n", EXAMPLE_NAME);
	tr_close(&socket);
	
	return 0;
}
