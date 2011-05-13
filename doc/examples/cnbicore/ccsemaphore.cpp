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

#include <libcnbicore/CcSemaphore.hpp>
#include <libcnbicore/CcThread.hpp>
#include <libcnbicore/CcTime.hpp>
#include <stdio.h>

using namespace std;

class Simis : public CcThread {
	public:
		Simis(CcSemaphore *ptr) {
			this->sem = ptr;
		}

		virtual void Main(void) {
			while(IsRunning()) {
				cout << "Wait1: " << this->sem->TryWait() << endl;
				cout << "Wait2: " << this->sem->TryWait() << endl;
				printf("Simis is using the shared resource!\n");
				sleep(1);
				this->sem->Post();
				sleep(1);
			}
		}

	public:
		CcSemaphore *sem;
};


class Michele : public CcThread {
	public:
		Michele(CcSemaphore *ptr) {
			this->sem = ptr;
		}

		virtual void Main(void) {
			while(IsRunning()) {
				this->sem->Wait();
				printf("Michele is using the shared resource!\n");
				sleep(2);
				this->sem->Post();
				sleep(1);
			}
		}
	
	public:
		CcSemaphore *sem;
};

class Tito : public CcThread {
	public:
		Tito(CcSemaphore *ptr) {
			this->sem = ptr;
		}

		virtual void Main(void) {
			while(IsRunning()) {
				this->sem->Wait();
				printf("Tito is using the shared resource!\n");
				sleep(3);
				this->sem->Post();
				sleep(1);
			}
		}
	
	public:
		CcSemaphore *sem;
};



int main (void) {
	CcSemaphore sync(1);
	

	CcSemaphore temp;
	cout << "Wait (1): " << temp.TryWait() << endl;
	cout << "Wait (0): " << temp.TryWait() << endl;
	temp.Post();
	cout << "Wait (1): " << temp.TryWait() << endl;
	cout << "Wait (0): " << temp.TryWait() << endl;

	CcTime::Sleep(5000);

	Michele michele(&sync);
	Simis simis(&sync);
	Tito tito(&sync);

	tito.Start();
	michele.Start();
	simis.Start();

	sleep(20);
	michele.Stop();
	simis.Stop();
	tito.Stop();


	michele.Join();
	simis.Join();
	tito.Join();
	return 0;
}
