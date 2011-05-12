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

#include "tr_net.h"
#include "tr_tcp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define EXAMPLE_NAME "[tr_tcpclient]"

int main(int argc, char * const argv[]) {
	if(argc < 3) {
		printf("Usage: tr_tcpclient ADDRESS PORT\n");
		return 2;
	}

	int i = 0;
	int bytes_send = 0;

	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket_default(&socket);
	tr_tcpclient(&socket);
	
	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);

	printf("%s Connecting socket\n", EXAMPLE_NAME);
	while(tr_connected(&socket) != 0) {
		tr_connect(&socket, argv[1], argv[2]);
		sleep(1);
	}
	/* Same thing
	while(1) {
		int status = tr_connect(&socket, argv[1], argv[2]);
		if(status == 0)
			break;
		sleep(1);
	}
	*/
	
	
	printf("%s Local TCP socket:  %s:%u [client]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);
	printf("%s Remote TCP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.remote.address, socket.remote.port);
	

	for(i = 0; i < 10; i++) {
		bytes_send = tr_send(&socket, "Can you read me?");
		printf("%s Sending: %d bytes\n", EXAMPLE_NAME, bytes_send);
		
		if(bytes_send == -1) {
			printf("%s Server dropped the connection [send]\n", EXAMPLE_NAME);
			break;
		}
		
		if(tr_recv(&socket) == -1) {
			printf("%s Server dropped the connection [recv]\n", EXAMPLE_NAME);
			break;
		}
		printf("%s Received: %s\n", EXAMPLE_NAME, socket.buffer);
		
		sleep(0.40);
	}

	printf("%s Closing socket\n", EXAMPLE_NAME);
	tr_close(&socket);

	return 0;
}
