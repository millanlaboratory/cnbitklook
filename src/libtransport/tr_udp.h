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

#ifndef UDP_H
#define UDP_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "tr_types.h"

/*! \file tr_udp.h 
 *	\brief Low-level UDP functionalities
 */ 

//! Initialize the tr_socket as a UDP server socket
void tr_udpserver(tr_socket* sock);

//! Initialize the tr_socket as a UDP client socket
void tr_udpclient(tr_socket* sock);

//! Receive as UDP
int tr_recvudp(tr_socket* sock, void* buffer);

//! Send as UDP
int tr_sendudp(tr_socket* sock, void* buffer, size_t bsize);

#ifdef __cplusplus
}
#endif

#endif
