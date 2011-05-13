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

#ifndef CCSEMAPHORE_CPP
#define CCSEMAPHORE_CPP

#include "CcSemaphore.hpp"

CcSemaphore::CcSemaphore(unsigned int count) {
	sem_init(&this->_semaphore, 0, count);
}

CcSemaphore::~CcSemaphore(void) {
	sem_destroy(&this->_semaphore);
}
		
void CcSemaphore::Wait(void) {
	sem_wait(&this->_semaphore);
}

void CcSemaphore::Post(void) {
	sem_post(&this->_semaphore);
}

bool CcSemaphore::TryWait(void) {
	if(sem_trywait(&this->_semaphore) == 0)
		return true;
	else
		return false;
}

#endif
