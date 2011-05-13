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
 
#include "tr_names.h"
#include "tr_types.h"
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
