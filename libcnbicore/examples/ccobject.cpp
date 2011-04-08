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

#include "CcObject.hpp"

using namespace std;

int main(void) {
	CcObject *lonely = new CcObject();
	lonely->SetName("lonely");
	lonely->DumpInfo();
	
	CcObject *pool[5];
	for(unsigned int j = 0; j < 5; j++) {
		pool[j] = new CcObject();
		pool[j]->SetName("pooled");
	}
	pool[0]->DumpInfo();

	CcObject spool[5];
	for(unsigned int j = 0; j < 5; j++) 
		spool[j].SetName("spooled");
	
	spool[0].DumpInfo();

	for(unsigned int j = 0; j < 5; j++)
		delete pool[j];

	lonely->DumpInfo();

	delete lonely;
}
