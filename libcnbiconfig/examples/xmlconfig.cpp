/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiconfig library

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

#include "CCfgXMLConfig.hpp"
#include <iostream>

using namespace std;

int main(void) {
	try {
		CCfgXMLConfig xml("../extra/example.xml");
		xml.Root()->Go("offline")->Go("mi_offline")->Go("description")->GetRaw();
		xml.Root()->Go("offline")->Go("mi_offline")->Go("taskset")->GetRaw();
		xml.Dump();
		
		xml.Root()->Go("offline")->Go("mi_offline")->SetBranch();
		xml.Branch()->Go("description")->GetRaw();
		xml.Branch()->Go("taskset")->GetRaw();
		cout << "got: " << xml.Branch()->Go("taskset")->Get().Key() << endl;
		xml.Dump();

		xml.Root()->Go("taskset")->Go("mirhlh")->Go("class")->SetBranch();
		string key = xml.Branch()->Get().Key();
		string id = xml.Branch()->Get().Id();
		int value = xml.Branch()->Get().Int();
		cout << "got: " << id << "/" << key << "=" << value << endl;
		
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
	}
	return 0;
}
