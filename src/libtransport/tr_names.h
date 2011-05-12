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

#ifndef NAMES_H
#define NAMES_H

#ifdef __cplusplus
extern "C" {
#endif 

/*! \file tr_names.h 
 *	\brief Name resolving functions
 */ 

/*! \brief Get the maximum address lenght
 *
 * Simply return (INET_ADDRSTRLEN+1)
 *
 * @return the maximum address lenght
 */
unsigned int tr_getaddrlen(void);

/*! \brief Resolve named host
 *
 * @param hostname The named host (i.e.: menace)
 * @param address The address (i.e.: 10.66.77.1)
 * @return 0 on success, -1 otherwise
 */
int tr_resolve(char* hostname, char* address);

#ifdef __cplusplus
}
#endif

#endif
