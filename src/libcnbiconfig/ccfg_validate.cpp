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
		config->ImportFileEx(filename);
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
		config->RootEx()->QuickStringEx("recording/experimenter") << endl;
	cout << "  Location:     " << 
		config->RootEx()->QuickStringEx("recording/location") << endl;
	cout << "  Experiment:   " << 
		config->RootEx()->QuickStringEx("recording/experiment") << endl;
	cout << "  Project:      " << 
		config->RootEx()->QuickStringEx("recording/project") << endl;
	cout << "  Date:         " << 
		config->RootEx()->QuickStringEx("recording/date") << endl;


	return isvalid;
}
