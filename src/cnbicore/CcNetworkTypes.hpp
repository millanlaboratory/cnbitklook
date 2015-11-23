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

#ifndef CCNETWORKTYPES_HPP
#define CCNETWORKTYPES_HPP

#include <transport/tr_types.h>
#include <map>
#include <string>

class CcStreamer;

typedef std::string CcIp;
typedef std::string CcPort;
typedef unsigned int CcPortUInt;
typedef std::string CcAddress;
typedef std::string CcHostname;

typedef std::map<int, CcStreamer*> CcStreamerMap;
typedef std::map<int, CcStreamer*>::iterator CcStreamerMapIt;
typedef std::map<int, CcStreamer*>::const_iterator CcStreamerMapCIt;

typedef std::map<int, tr_socket*> CcSocketMap;
typedef std::map<int, tr_socket*>::iterator CcSocketMapIt;
typedef std::map<int, tr_socket*>::const_iterator CcSocketMapCIt;

#endif
