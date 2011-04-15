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
 
#include "names.h"
#include "types.h"
#include <netinet/in.h>
#include <string.h>

unsigned int tr_getaddrlen(void) { 
	return INET_ADDRSTRLEN + 1;
}

int tr_resolve(char* hostname, char* address) {
	struct hostent *host;
	struct in_addr h_addr;
	
	host = gethostbyname(hostname);
	if(host == 0)
		return -1;

	h_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
	memcpy(address, inet_ntoa(h_addr), INET_ADDRSTRLEN + 1);
	
	return 0;
}
