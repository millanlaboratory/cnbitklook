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

#include "CcTime.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(void) {
	string timestamp;
	CcTime::Daystamp(&timestamp);
	cout << timestamp << endl;
	CcTime::Datetime(&timestamp);
	cout << timestamp << endl;
	CcTime::Sleep(1.00f);
	CcTime::Timestamp(&timestamp);
	cout << timestamp << endl;
	
	CcTimeValue tic;
	for(int i = 0; i < 20; i++) {
		CcTime::Tic(&tic);
		CcTime::Sleep(1234.00f);
		printf("Waited: %f\n", CcTime::Toc(&tic));
	}
	
	double pause = 0.0f;
	for(int i = 0; i < 20; i++) {
		pause = CcTime::Sleep(1000.00f, 1000.10f);
		cout << pause << endl;
	}
	return 0;
}
