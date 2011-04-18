/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ClAcqLang.hpp"
#include <iostream>

using namespace std;

int main(void) {
	string label;
	string filegdf, filemat, filelog;
	int id = 0;

	ClAcqLang acq;
	cout << acq.OpenXDF("test.gdf", "mylog", "classifier=test.mat") << endl;
	cout << acq.IsOpenXDF(acq.message->buffer, &filegdf, &filelog, &filemat) << endl;
	cout << "--> Message:  " << acq.message->buffer << endl;
	cout << "--> File XDF: " << filegdf << endl;
	cout << "--> Line LOG: " << filemat << endl;
	
	cout << acq.OpenXDF("test.gdf", "mylog", "classifier=test.mat rejection=0.60") 
		<< endl;
	cout << acq.IsOpenXDF(acq.message->buffer, &filegdf, &filelog, &filemat) << endl;
	cout << "--> Message:  " << acq.message->buffer << endl;
	cout << "--> File XDF: " << filegdf << endl;
	cout << "--> Line LOG: " << filemat << endl;
	
	cout << acq.CloseXDF() << endl;
	cout << acq.IsCloseXDF(acq.message->buffer) << endl;

	cout << acq.Error(acq.NDFLimitReached) << endl;
	cout << acq.IsError(acq.message->buffer, &id) << endl;
	
	cout << acq.Ok() << endl;
	cout << acq.IsOk(acq.message->buffer) << endl;

	return 0;
}
