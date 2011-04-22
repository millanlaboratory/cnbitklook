/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libxmlconfig library

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
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>

/*! \brief Tools class.
 *
 * It exposes some static methods to perform data conversion.
 */
class Tools {
	public:	
		static char* itoa(int value, char* result, int base = 10);
		static int ftoa(float value, char* result);
		static float atof(const char* value);
		static std::string itos(int value);
};

#endif
