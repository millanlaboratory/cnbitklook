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

#ifndef CCTHREAD_HPP
#define CCTHREAD_HPP

#include "CcSemaphore.hpp"
#include "CcObject.hpp"
#include <pthread.h>

/*! \brief pthread wrapper
 *
 * Simple thread implementation, does not do much more than wrapping pthread and
 * providing a pure virtual method to implement the thread's main loop.
 */
class CcThread : public CcObject {
	public:
		CcThread(void);
		virtual ~CcThread(void);
		virtual void Main(void) = 0;
		void Start(void);
		void Stop(void);
		void Wait(void);
		int Join(void);
		bool IsRunning(void);
		bool IsStarted(void);
	protected:
		virtual void pBeforeStart(void) {};
		virtual void pAfterStart(void) {};
		virtual void pBeforeStop(void) {};
		virtual void pAfterStop(void) {};
	
	private:
		CcSemaphore _semstatus, _semthread;
		bool _running, _started;
		pthread_t _thread;
};

#endif
