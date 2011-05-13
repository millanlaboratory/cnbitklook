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

#ifndef TYPES_H
#define TYPES_H

#include <arpa/inet.h>
#include <netdb.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file tr_types.h 
 *	\brief Socket and host data types
 */ 

#define	TR_SOCKET_UNDEF			0	

#define	TR_PROTO_NOTSUPPORTED	-3
#define	TR_PROTO_UNDEF			0
#define	TR_PROTO_TCP			1
#define	TR_PROTO_UDP			2

#define TR_HOST_VALID			1
#define TR_HOST_NOTVALID		0

#define TR_BYTES_NONE			-1
#define TR_BYTES_ERROR			0

#define	TR_TYPE_UNDEF			0
#define	TR_TYPE_SERVER 			1
#define	TR_TYPE_ENDPOINT		2
#define	TR_TYPE_CLIENT			3

/*! \brief libtransport host 
 * This structure holds whatever identifies an host
 */
typedef struct tr_host_struct {
	//! Address (e.g.: 127.0.0.1)
	char address[INET_ADDRSTRLEN + 1];

	//! Port (e.g.: 8000)
	short unsigned int port;

	//! States wether the host has ben set or not
	int valid;
} tr_host;

/*! \brief libtransport socket
 * This structure holds the socket, the sockaddr_in address
 * and the buffer of size TR_BUFFER_LENGTH
 */
typedef struct tr_socket_struct {
	//! Socket descriptor
	int fd;
	
	//! Socket address
	struct sockaddr_in address;
	struct sockaddr_storage address_endpoint;

	//! Buffer 
	//char buffer[TR_BUFFER_LENGTH];
	void* buffer;
	size_t bsize;

	//! Maximum number of connections
	unsigned int max_conns;

	//! Socket protocol 
	int protocol;
	
	//! Socket type
	int type;

	//! Socket info for connecting and binding
	struct addrinfo info_results;
	
	//! Socket info
	struct addrinfo *info;

	//! Host informations associated to the current socket
	tr_host local;
	tr_host remote;

	//! Maximum size of message
	size_t maxbsize;
} tr_socket;

#ifdef __cplusplus
}
#endif

#endif
