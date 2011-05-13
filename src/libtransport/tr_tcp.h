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

//! Receive as TCP on the internal buffer
int tr_recvtcp(tr_socket* sock);

//! Receive as TCP
int tr_recvtcpb(tr_socket* sock, void* buffer, size_t bsize);

//! Send as TCP
int tr_sendtcp(tr_socket* sock, const void* message, size_t bsize);

#ifdef __cplusplus
}
#endif

#endif
