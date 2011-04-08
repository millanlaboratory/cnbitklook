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

#ifndef CCTHREADSAFE_CPP
#define CCTHREADSAFE_CPP

#include "CcThreadSafe.hpp"

template <class T> 
CcThreadSafe<T>::CcThreadSafe(void) {
	this->_semdata.Post();
}
		
template <class T> 
CcThreadSafe<T>::CcThreadSafe(T value) {
	this->_data = value;
	this->_semdata.Post();
}

template <class T> 
void CcThreadSafe<T>::Set(T value) {
	this->_semdata.Wait();
	this->_data = value;
	this->_semdata.Post();
}

template <class T> 
T CcThreadSafe<T>::Get(void) {
	this->_semdata.Wait();
	T value = this->_data;
	this->_semdata.Post();
	return value;
}
#endif
