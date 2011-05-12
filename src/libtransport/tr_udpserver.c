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
#include "tr_udp.h"
#include <stdio.h>
#include <unistd.h>

#define EXAMPLE_NAME "[tr_udpserver]"

int main(void) { 
	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket_default(&socket);
	tr_udpserver(&socket);
	
	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);
	
	printf("%s Binding socket\n", EXAMPLE_NAME);
	tr_bind(&socket, "8000");

	printf("%s Local UDP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);
	
	printf("%s Entering recv loop\n", EXAMPLE_NAME);
	while(1) {
		if(tr_recv(&socket) == -1) {
			printf("%s Local socket error\n", EXAMPLE_NAME);
			break;
		}
		printf("%s Received: %s\n", EXAMPLE_NAME, (char*)socket.buffer);
		printf("%s Remote TCP socket: %s:%u [endpoint]\n", EXAMPLE_NAME, 
				socket.remote.address, socket.remote.port);
	}

	tr_close(&socket);

	return 0;
}

