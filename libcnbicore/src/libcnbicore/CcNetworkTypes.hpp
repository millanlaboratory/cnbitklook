/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

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

#ifndef CCNETWORKTYPES_HPP
#define CCNETWORKTYPES_HPP

#include <libtransport/types.h>
#include <map>
#include <vector>
#include <string>
#include <utility>

// Forward definition
class CcEndpoint;
class CcStreamer;

typedef std::string CcIp;
typedef std::string CcPort;
typedef unsigned int CcPortUInt;
typedef std::string CcAddress;
typedef std::string CcHostname;
typedef std::string CcPlugname;

typedef std::map<CcAddress, CcEndpoint*> CcEndpointMap;
typedef std::map<CcAddress, tr_socket*> CcSocketMap;
typedef std::vector<CcAddress> CcEndpointList;
typedef std::map<CcAddress, CcStreamer*> CcStreamerMap;

typedef CcEndpointMap::iterator CcEndpointMapIt;
typedef CcSocketMap::iterator CcSocketMapIt;
typedef CcEndpointMap::const_iterator CcEndpointMapCIt;
typedef CcSocketMap::const_iterator CcSocketMapCIt;
typedef CcEndpointList::iterator CcEndpointListIt;
typedef CcEndpointList::const_iterator CcEndpointListCIt;
typedef CcStreamerMap::iterator CcStreamerMapIt;
typedef CcSocketMap::iterator CcSocketMapIt;


std::string CcAddress2string(const CcAddress address);

#endif
