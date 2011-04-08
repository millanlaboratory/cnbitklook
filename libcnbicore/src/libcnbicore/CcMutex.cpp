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
#ifndef CCMUTEX_CPP
#define CCMUTEX_CPP

#include "CcMutex.hpp"

CcMutex::CcMutex(void) {
	CcObject::SetName("CcMutex");
	pthread_mutex_init(&this->_mutex, NULL);
}

CcMutex::~CcMutex(void) {
	pthread_mutex_destroy(&this->_mutex);
}
		
void CcMutex::Lock(void) {
	pthread_mutex_lock(&this->_mutex);
}

void CcMutex::Release(void) {
	pthread_mutex_unlock(&this->_mutex);
}

bool CcMutex::TryLock(void) {
	if(pthread_mutex_trylock(&this->_mutex) == 0)
		return true;
	else
		return false;
}

#endif
