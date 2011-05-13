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

#ifndef NDFTYPES_C
#define NDFTYPES_C

#include "ndf_types.h"

size_t ndf_sizeof(ndf_typeid id) {
	switch(id) {
		case NDF_UINT8:
			return sizeof(uint8_t);
		case NDF_UINT16:
			return sizeof(uint16_t);
		case NDF_UINT32:
			return sizeof(uint32_t);
		case NDF_UINT64:
			return sizeof(uint64_t);
		case NDF_INT8:
			return sizeof(int8_t);
		case NDF_INT16: 
			return sizeof(int16_t);
		case NDF_INT32:
			return sizeof(int32_t);
		case NDF_INT64:
			return sizeof(int64_t);
		case NDF_FLOAT:
			return sizeof(float);
		case NDF_DOUBLE:
			return sizeof(double);
		case NDF_TIMEVAL:
			return sizeof(struct timeval);
		case NDF_DOUBLETIME:
			return sizeof(double);
	}
	return 0;
}

#endif
