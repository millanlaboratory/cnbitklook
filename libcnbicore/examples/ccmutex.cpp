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

#include "CcCore.hpp"
#include "CcTime.hpp"
#include "CcMutex.hpp"
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
