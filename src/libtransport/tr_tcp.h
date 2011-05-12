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

#ifndef TCP_H
#define TCP_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "tr_types.h"

/*! \file tr_tcp.h 
 *	\brief Low-level TCP functionalities
 */ 

//! Initialize the tr_socket as a TCP server socket
void tr_tcpserver(tr_socket* sock);

//! Initialize the tr_socket as a TCP endpoint socket
void tr_tcpendpoint(tr_socket* sock);

//! Initialize the tr_socket as a TCP client socket
void tr_tcpclient(tr_socket* sock);

//! Receive as TCP
int tr_recvtcp(tr_socket* sock, void* buffer);

//! Send as TCP
int tr_sendtcp(tr_socket* sock, void* message, size_t bsize);

#ifdef __cplusplus
}
#endif

#endif
