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

#include <cnbicore/CcCore.hpp>
#include <cnbicore/CcTime.hpp>
#include <cnbicore/CcMutex.hpp>
#include <stdio.h>

using namespace std;

int main(void) {
	bool result = false;
	CcMutex mutex;
	
	printf("[mutex.cpp] Locking and entering loop\n");
	mutex.Lock();
	for(int i = 0; i < 4; i++) {
		result = mutex.TryLock();
		printf(">> %d\n", result);
	}
	printf("[mutex.cpp] Releasing and entering loop\n");
	mutex.Release();
	for(int i = 0; i < 4; i++) {
		result = mutex.TryLock();
		printf(">> %d\n", result);
	}

	CcCore::Status();
	return 0;
}
