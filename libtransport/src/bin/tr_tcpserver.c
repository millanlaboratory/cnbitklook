/*
    Copyright (C) 2009 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libtransport library

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

#include "net.h"
#include "tcp.h"
#include <stdio.h>
#include <unistd.h>

#define EXAMPLE_NAME "[tr_tcpserver]"

int main(int argc, char * const argv[]) {
	if(argc < 2) {
		printf("Usage: tr_tcpserver PORT\n");
		return 2;
	}

	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket_default(&socket);
	tr_tcpserver(&socket);

	tr_socket endpoint;
	
	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);

	printf("%s Binding socket\n", EXAMPLE_NAME);
	tr_bind(&socket, argv[1]);
	
	printf("%s Local TCP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);

	printf("%s Listening\n", EXAMPLE_NAME);
	tr_listen(&socket);
	

	printf("%s Entering accept loop\n", EXAMPLE_NAME);
	while(1) {
		tr_init_socket_default(&endpoint);
		tr_tcpendpoint(&endpoint);
		printf("%s Waiting for endpoint\n", EXAMPLE_NAME);
		tr_accept(&socket, &endpoint);
	
		printf("%s Remote TCP socket: %s:%u [endpoint]\n", EXAMPLE_NAME, 
				endpoint.remote.address, endpoint.remote.port);

		tr_send(&endpoint, "Welcome Mr. Client");
		while(tr_recv(&endpoint) != -1) {
			printf("%s Received: %s\n", EXAMPLE_NAME, endpoint.buffer);
			if(tr_send(&endpoint, "Thanks for your message") == -1)
				break;
		}
		printf("%s Endpoint dropped the connection\n", EXAMPLE_NAME);
		tr_close(&endpoint);
	}

	printf("%s Closing socket\n", EXAMPLE_NAME);
	tr_close(&socket);
	
	return 0;
}
