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

#ifndef NET_H
#define NET_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "tr_types.h"


/*! \file tr_net.h 
 *	\brief Abstraction layer for TCP and UDP networking 
 *
 *	You should use the generic functions here defined unless you need to work at
 *	a lower level.
 */ 

/*! \brief Initialize the tr_host structure
 * 
 * Default values applied here.
 *
 * @param host The host
 */
void tr_init_host(tr_host* host);

/*! \brief Initialize the tr_socket structure
 *
 * Full customization
 *
 * @param sock The socket
 * @param bsize Buffer size
 * @param max_conns Maximum number of connections
 */
void tr_init_socket(tr_socket* sock, size_t bsize, unsigned int max_conns);

/*! \brief Open the socket
 * 
 * Setup the socket according to its role (server, client or endpoint) and its
 * protocol (TPC or UDP).
 *
 * @param sock The socket
 * @return 0 if successfull, -1 otherwise
 */
int tr_open(tr_socket* sock);

/*! \brief Close the socket. 
 *
 * Reset the file descriptor and the socket role.
 *
 * @param sock The socket
 * @return -1 if socket is not initialized at TCP/UDP, 0 if successfull
 */
int tr_close(tr_socket* sock);

/*! \brief Frees the socket buffer
 *
 * The internal buffer is not freed by tr_close() to allow reuse of the socket
 *
 * @param sock The socket
 */
void tr_free(tr_socket* sock);

/*! 
 * \brief Check if tr_socket has a valid file descriptor
 *
 * @param sock The socket
 * @return 0 if not connected/bound, the file descriptor otherwise
 */
int tr_check(tr_socket* sock);

/*!  \brief Bind the socket
 * 
 * This method binds the socket to a port.
 *
 * @param sock The socket
 * @param port The port
 * @return 0 if successfull, -1 otherwise
 */
int tr_bind(tr_socket* sock, const char* port);

/*! \brief Start listening
 * 
 * @param sock The socket
 * @return 0 if successfull, -1 otherwise
 */
int tr_listen(tr_socket* sock);

/*! \brief Accept a new client
 * 
 * @param sock The socket
 * @param endpoint The socket client, initialized as endpoint
 * @return 0 if successfull, -1 otherwise
 */
int tr_accept(tr_socket* sock, tr_socket* endpoint);

/*! \brief Connect to server
 * 
 * @param sock The socket
 * @return 0 if successfull, -1 otherwise
 */
int tr_connect(tr_socket* sock, const char* host, const char* port);

/*! \brief Check if connected
 * 
 * @param sock The socket
 * @return 0 if connected, -1 otherwise
 */
int tr_connected(tr_socket* sock);

/*! \brief Receive in internal buffer
 * 
 * @param sock The socket
 * @return -1 if the connection is not valid, the number of received bytes.
 * otherwise
 */
int tr_recv(tr_socket* sock);

/*! \brief Receive in external buffer
 * 
 * @param sock The socket
 * @return -1 if the connection is not valid, the number of received bytes
 * otherwise
 */
int tr_recvb(tr_socket* sock, void* buffer, size_t bsize);

/*! \brief Send data
 * 
 * @param sock The socket
 * @return -1 if the connection is not valid, the number of sent bytes
 * otherwise
 */
int tr_send(tr_socket* sock, const char* buffer);

/*! \brief Send non-char data
 *
 * @param sock The socket
 * @return -1 if the connection is not valid, the number of sent bytes
 * otherwise
 */
int tr_sendb(tr_socket* sock, void* buffer, size_t bsize);

/*! \brief Setup the socket for async operation
 * 
 * This is needed to avoid blocking on receive
 *
 * @param sock The socket
 * @return 0 if successfull, -1 otherwise
 */
int tr_set_nonblocking(tr_socket *socket, int value);

/*! \brief Get local host informations
 *
 * Set local ip and port in the tr_host structure
 *
 * @param sock The socket
 * @return 0 if successfull, -1 otherwise
 */
int tr_gethost_local(tr_socket* sock, tr_host *host);

/*! \brief Get remote host informations
 * 
 * Set remote ip and port in the tr_host structure
 *
 * @param sock The socket
 * @return 0 if successfull, -1 otherwise
 */
int tr_gethost_remote(tr_socket* sock, tr_host *host);

void tr_getmaxbsize(tr_socket* sock);

#ifdef __cplusplus
}
#endif

#endif
