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
#include <libcnbicore/CcTime.hpp>
#include <libcnbicore/CcMutex.hpp>
#include <libcnbicore/CcThread.hpp>
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
