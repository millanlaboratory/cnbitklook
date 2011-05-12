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
 
#include "tr_tcp.h"
#include <string.h>

void tr_tcpserver(tr_socket* sock) {
	sock->protocol = TR_PROTO_TCP;
	sock->type = TR_TYPE_SERVER;
}

void tr_tcpendpoint(tr_socket* sock) {
	sock->protocol = TR_PROTO_TCP;
	sock->type = TR_TYPE_ENDPOINT;
}

void tr_tcpclient(tr_socket* sock) {
	sock->protocol = TR_PROTO_TCP;
	sock->type = TR_TYPE_CLIENT;
}

int tr_recvtcp(tr_socket* sock) {
	return tr_recvtcpb(sock, sock->buffer, sock->bsize);
}

int tr_recvtcpb(tr_socket* sock, void* buffer, size_t bsize) {
	if(sock->protocol != TR_PROTO_TCP)
		return TR_PROTO_NOTSUPPORTED;

	return recv(sock->fd, buffer, bsize, 0);
}

int tr_sendtcp(tr_socket* sock, const void* buffer, size_t bsize) {
	if(sock->protocol != TR_PROTO_TCP)
		return TR_PROTO_NOTSUPPORTED;
	
	return send(sock->fd, buffer, bsize, MSG_NOSIGNAL);
}

/*
size_t bytes = 0, tbytes = 0, csize = 0;
if(bsize <= sock->maxbsize) {
	tbytes = send(sock->fd, buffer, bsize, MSG_NOSIGNAL);
} else { 
	while(1) {
		csize = tbytes + sock->maxbsize < bsize ? sock->maxbsize : bsize - tbytes;

		bytes = send(sock->fd, buffer + tbytes, csize, MSG_NOSIGNAL);
		if(bytes != -1) {
			tbytes += bytes;
		}
		if(tbytes == bsize)
			break;
	}
}
return tbytes;
*/
