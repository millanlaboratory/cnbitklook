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

#ifndef CCMUTEX_HPP
#define CCMUTEX_HPP

#include "CcObject.hpp"
#include <pthread.h>

/*! \brief Wrapper for pthread mutex
 *
 */
class CcMutex : public CcObject {
	public:
		CcMutex(void);
		~CcMutex(void);

		void Lock(void);
		void Release(void);
		bool TryLock(void);

	protected:
		pthread_mutex_t _mutex;

};

#endif
