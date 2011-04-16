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

#include "CCfgConfig.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: ccfg_validate XMLFILE\n");
		return -1;
	}
		
	CCfgConfig* config;
	string filename(argv[1]);
	try {
		config = new CCfgConfig();
		config->LoadFile(filename);
	} catch(XMLException e) {
		cout << "[ccfg_validate] Exception: " << e << endl;
		return -2;
	}
		
	bool isvalid =config->Validate(); 
	if(!isvalid) {
		cout << "[ccfg_validate] XML file is not valid" << endl;
		return isvalid;
	}
	
	cout << "[ccfg_validate] XML file is valid" << endl;
	cout << "  Experimenter: " << 
		config->Root()->QuickString("recording/experimenter") << endl;
	cout << "  Location:     " << 
		config->Root()->QuickString("recording/location") << endl;
	cout << "  Experiment:   " << 
		config->Root()->QuickString("recording/experiment") << endl;
	cout << "  Project:      " << 
		config->Root()->QuickString("recording/project") << endl;
	cout << "  Date:         " << 
		config->Root()->QuickString("recording/date") << endl;


	return isvalid;
}
