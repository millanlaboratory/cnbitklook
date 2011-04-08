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

#ifndef CCDESERIALIZER_HPP
#define CCDESERIALIZER_HPP

#include "CcStreamer.hpp"

/*! \brief Interface for classes that deserialize data from CcStreamer
 */
class CcDeserializer {
	public:
		virtual bool Deserialize(string* buffer) = 0;
		virtual bool Deserialize(CcStreamer* stream, 
				CcStreamerDirection direction) = 0;
};
#endif
