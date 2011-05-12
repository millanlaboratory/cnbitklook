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

#define EXAMPLE_NAME "[tr_udpclient]"

int main(void) { 
	int i = 0;

	printf("%s Starting up\n", EXAMPLE_NAME);
	tr_socket socket;
	tr_init_socket(&socket, 1024, 1);
	tr_udpclient(&socket);

	printf("%s Opening socket\n", EXAMPLE_NAME);
	tr_open(&socket);

	printf("%s Connecting\n", EXAMPLE_NAME);
	tr_connect(&socket, "127.0.0.1", "8000");
	
	printf("%s Local UDP socket: %s:%u [client]\n", EXAMPLE_NAME, 
			socket.local.address, socket.local.port);
	printf("%s Remote UDP socket: %s:%u [server]\n", EXAMPLE_NAME, 
			socket.remote.address, socket.remote.port);
	
	printf("%s Entering send loop\n", EXAMPLE_NAME);
	for(i = 0; i < 5; i++) {
		printf("%s Sending: %d\n", EXAMPLE_NAME, 
				tr_send(&socket, "Can you read me?"));
		sleep(1);
	}

	tr_close(&socket);
	return 0;
}

