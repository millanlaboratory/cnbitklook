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
 
#include "tr_net.h"
#include "tr_tcp.h"
#include "tr_udp.h"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void tr_init_host(tr_host* host) {
	memset(host->address, 0, INET_ADDRSTRLEN + 1);
	host->port = 0;
	host->valid = TR_HOST_NOTVALID;
}

void tr_init_socket(tr_socket* sock, size_t bsize, unsigned int max_conns) {
	memset(sock, 0, sizeof(sock));
	sock->fd = TR_SOCKET_UNDEF;
	sock->type = TR_TYPE_UNDEF;
	sock->protocol = TR_PROTO_UNDEF;
	
	sock->max_conns = max_conns;
	
	sock->bsize = bsize;
	sock->buffer = malloc(bsize * sizeof(char));
	memset(sock->buffer, 0, sock->bsize);
	
	memset(&(sock->address), 0, sizeof(sock->address));
	memset(&(sock->info_results), 0, sizeof(sock->info_results));
	memset(&(sock->address_endpoint), 0, sizeof(sock->address_endpoint));

	tr_init_host(&(sock->local));
	tr_init_host(&(sock->remote));

	sock->maxbsize = 0;
}

int tr_open(tr_socket* sock) {
	switch(sock->protocol) {
		case TR_PROTO_TCP:
			sock->info_results.ai_family = AF_UNSPEC;
			sock->info_results.ai_socktype = SOCK_STREAM;
			if(sock->type != TR_TYPE_CLIENT)
				sock->info_results.ai_flags = AI_PASSIVE;
			break;
		case TR_PROTO_UDP:
			sock->info_results.ai_family = AF_UNSPEC;
			sock->info_results.ai_socktype = SOCK_DGRAM;
			if(sock->type != TR_TYPE_CLIENT) 
				sock->info_results.ai_flags = AI_PASSIVE;
			break;
		default:
			return -1;
	}
	return 0;
}

int tr_close(tr_socket* sock) {
	int status = 0;
	if(sock->fd) {
		status = close(sock->fd);
		sock->fd = TR_SOCKET_UNDEF;
		sock->type = TR_TYPE_UNDEF;
	}
	return status;
}

void tr_free(tr_socket* sock) {
	if(sock->buffer != 0) {
		free(sock->buffer);
		sock->buffer = 0;
	}
}

int tr_check(tr_socket* sock) {
	return sock->fd;
}

int tr_bind(tr_socket* sock, const char* port) {
	int retopt = 0;
	int bndret = 0;
	struct addrinfo *ai; 
	int opt = 1;

	if(getaddrinfo(NULL, port, &(sock->info_results), &ai) != 0)
		return -1;

	for(sock->info = ai; sock->info != NULL; sock->info = sock->info->ai_next) {
		sock->fd = socket(sock->info->ai_family, 
				sock->info->ai_socktype,
				sock->info->ai_protocol);
		if(sock->fd == -1) 
			continue;

		if(sock->protocol == TR_PROTO_TCP) {
			retopt = setsockopt(sock->fd, 
					SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
			if(retopt == -1) 
				return -1;
		}

		bndret = bind(sock->fd, sock->info->ai_addr, sock->info->ai_addrlen); 
		if(bndret == -1) {
			close(sock->fd);
			continue;
		}
		break;
	}
	freeaddrinfo(ai);

	// Fill local host structure
	tr_gethost_local(sock, &(sock->local));

	return bndret;
}

int tr_listen(tr_socket* sock) {
	if(!tr_check(sock))
		return -1;
	
	return listen(sock->fd, sock->max_conns);
}

int tr_accept(tr_socket* sock, tr_socket* endpoint) {
	if(sock->protocol != TR_PROTO_TCP) 
		return -3;

	unsigned int addrlen = sizeof(endpoint->address);
	endpoint->fd = accept(sock->fd, (struct sockaddr*)&sock->address_endpoint,
			&addrlen);
	endpoint->type = TR_TYPE_ENDPOINT;

	// Fill local and remote host structures
	tr_gethost_local(endpoint, &(endpoint->local));
	tr_gethost_remote(endpoint, &(endpoint->remote));
	tr_getmaxbsize(endpoint);
	
	return endpoint->fd;
}

int tr_connect(tr_socket* sock, const char* host, const char* port) {
	struct addrinfo *ai;
	int conopt = 0;

	if(getaddrinfo(host, port, &(sock->info_results), &ai) != 0)
		return -1;
	
	for(sock->info = ai; sock->info != NULL; sock->info = sock->info->ai_next) {
		sock->fd = socket(sock->info->ai_family,
				sock->info->ai_socktype,
				sock->info->ai_protocol);
		if (sock->fd == -1) 
			continue;

		if(sock->protocol == TR_PROTO_TCP) {
			conopt = connect(sock->fd, 
					sock->info->ai_addr,
					sock->info->ai_addrlen);
			if (conopt == -1) {
				close(sock->fd);
				continue;
			}
		}
		break;
	}

	tr_getmaxbsize(sock);

	//TODO check res here
	tr_gethost_local(sock, &(sock->local));
	tr_gethost_remote(sock, &(sock->remote));

	return conopt;
}

int tr_connected(tr_socket* sock) {
	return tr_send(sock, "\0");
}


ssize_t tr_recv(tr_socket* sock) {
	switch(sock->protocol) {
		case TR_PROTO_TCP:
			return tr_recvtcp(sock);
			break;
		case TR_PROTO_UDP:
			return tr_recvudp(sock);
			break;
		default:
			return -2;
	}
}

ssize_t tr_recvb(tr_socket* sock, void* buffer, size_t bsize) {
	switch(sock->protocol) {
		case TR_PROTO_TCP:
			return tr_recvtcpb(sock, buffer, bsize);
			break;
		case TR_PROTO_UDP:
			return tr_recvudpb(sock, buffer, bsize);
			break;
		default:
			return -2;
	}
}

ssize_t tr_send(tr_socket* sock, const char* buffer) {
	switch(sock->protocol) {
		case TR_PROTO_TCP:
			return tr_sendtcp(sock, (void*)buffer, strlen(buffer));
			break;
		case TR_PROTO_UDP:
			return tr_sendudp(sock, (void*)buffer, strlen(buffer));
		default:
			return -2;
	}
}

ssize_t tr_sendb(tr_socket* sock, void* buffer, size_t bsize) {
	switch(sock->protocol) {
		case TR_PROTO_TCP:
			return tr_sendtcp(sock, buffer, bsize);
			break;
		case TR_PROTO_UDP:
			return tr_sendudp(sock, buffer, bsize);
		default:
			return -2;
	}
}

int tr_set_nonblocking(tr_socket *socket, int value) {
	int oldflags = fcntl(socket->fd, F_GETFL, 0);
	if (oldflags < 0)
		return oldflags;

	if (value != 0)
		oldflags |= O_NONBLOCK;
	else
		oldflags &= ~O_NONBLOCK;
	return fcntl(socket->fd, F_SETFL, oldflags);
}

int tr_gethost_local(tr_socket* sock, tr_host *host) {
	const char* status = NULL;
	struct sockaddr addr;
	int addrlen = sizeof(addr);

	getsockname(sock->fd, 
			(struct sockaddr*)&addr, (socklen_t*)&addrlen);
	struct sockaddr_in *addr_ptr = (struct sockaddr_in*)&addr;
	
	host->port = ntohs(addr_ptr->sin_port);
	status = inet_ntop(AF_INET, 
			&(addr_ptr->sin_addr.s_addr), host->address, (socklen_t)addrlen);

	if(status == NULL)
		return -1;

	host->valid = TR_HOST_VALID;
	return 0;
}

int tr_gethost_remote(tr_socket* sock, tr_host *host) {
	const char* status = NULL;
	struct sockaddr addr;
	int addrlen = sizeof(addr);
	
	getpeername(sock->fd, 
			(struct sockaddr*)&addr, (socklen_t*)&addrlen);
	struct sockaddr_in *addr_ptr = (struct sockaddr_in*)&addr;
	
	host->port = ntohs(addr_ptr->sin_port);
	status = inet_ntop(AF_INET, 
			&(addr_ptr->sin_addr.s_addr), host->address, (socklen_t)addrlen);

	if(status == NULL)
		return -1;
	host->valid = TR_HOST_VALID;
	return 0;
}

void tr_getmaxbsize(tr_socket* sock) {
	socklen_t optlen = sizeof(sock->maxbsize);
	if(getsockopt(sock->fd, SOL_SOCKET, SO_SNDBUF, &sock->maxbsize, &optlen) < 0) {
		sock->maxbsize = 0;
	}
}
