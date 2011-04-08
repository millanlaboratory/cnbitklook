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
#include "CcMutex.hpp"
#include "CcThread.hpp"
#include <pthread.h>
#include <stdio.h>
#include <string>

using namespace std;

class Worker : public CcThread {
	public:
		virtual void Main(void) {
			while(this->IsRunning()) {
				printf("Hello world: %d\n", this->dt);
				sleep(this->dt);
			}
		}
	public:
		int dt;
};


class Simis : public CcThread {
	public:
		Simis(CcMutex *ptr) {
			this->mutex = ptr;
		}

		virtual void Main(void) {
			while(IsRunning()) {
				this->mutex->Lock();
				printf("Simis is using the shared resource!\n");
				sleep(1);
				this->mutex->Release();
				sleep(3);
			}
		}

	public:
		CcMutex *mutex;
};


class Michele : public CcThread {
	public:
		Michele(CcMutex *ptr) {
			this->mutex = ptr;
		}

		virtual void Main(void) {
			while(IsRunning()) {
				this->mutex->Lock();
				printf("Michele is using the shared resource!\n");
				sleep(1);
				this->mutex->Release();
				sleep(1);
			}
		}
	
	public:
		CcMutex *mutex;
};

class Tito : public CcThread {
	public:
		Tito(CcMutex *ptr) {
			this->mutex = ptr;
		}

		virtual void Main(void) {
			while(IsRunning()) {
				if(this->mutex->TryLock()) {
					printf("Tito is using the shared resource!\n");
					sleep(1);
					this->mutex->Release();
					sleep(10);
				} else {
					printf("Tito can not use the shared resource....\n");
					sleep(1);
				}
			}
		}
	
	public:
		CcMutex *mutex;
};



int main (void) {

	CcMutex sync;
		
	Michele michele(&sync);
	Simis simis(&sync);
	Tito tito(&sync);

	tito.Start();
	michele.Start();
	simis.Start();

	Worker *w1 = new Worker();
	Worker *w2 = new Worker();

	w1->dt = 1;
	w2->dt = 2;

	w1->Start();
	w2->Start();

	sleep(5);
	w1->Stop();
	w2->Stop();


	michele.Join();
	simis.Join();
	tito.Join();
	return 0;
}
