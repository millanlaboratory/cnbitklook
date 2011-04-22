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

#include "XMLTools.hpp"
#include "XMLException.hpp"
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <sstream>

char* Tools::itoa(int value, char* result, int base) {
	if(base < 2 || base > 36) { 
		*result = '\0'; 
		return result; 
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba\
				  9876543210123456789\
				  abcdefghijklmnopqrstuvwxyz"\
				  [35 + (tmp_value - value * base)];
	} while(value);

	if(tmp_value < 0) 
		*ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

int Tools::ftoa(float value, char* result) {
	return sprintf(result, "%.6f", value);
}

float Tools::atof(const char* value) {
	float retval;
	if(sscanf(value, "%f", &retval) != 1)
		throw XMLException("String does not contain single float", 
				__PRETTY_FUNCTION__);
	return retval;
}


std::string Tools::itos(int value) {
	std::stringstream out;
	out << value;
	return out.str();
}
