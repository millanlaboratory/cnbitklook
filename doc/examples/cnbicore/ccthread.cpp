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

#include <cnbicore/CcTime.hpp>
#include <cnbicore/CcMutex.hpp>
#include <cnbicore/CcThread.hpp>
#include <pthread.h>
#include <stdio.h>
#include <string>

using namespace std;

class AThread : public CcThread {
	public:
		virtual void Main(void) {
			while(this->IsRunning()) {
				printf("Hello world\n");
				CcTime::Sleep(1000);
			}
			printf("Finished...\n");
		}
};


int main (void) {
	AThread t;
	printf(">> Joining\n");
	t.Join(); 

	for(int i = 0; i < 2; i++) {
		printf(">> Starting iter=%d\n", i);
		t.Start();
		CcTime::Sleep(3000);
		printf(">> Stopping\n");
		t.Stop();
		printf(">> Joining\n");
		int status = t.Join(); 
		printf(">> Joined: %d\n", status);
		
		CcTime::Sleep(1000);
	}
	
	AThread* s = new AThread();
	s->Start();
	CcTime::Sleep(5000);
	delete s;

	return 0;
}
