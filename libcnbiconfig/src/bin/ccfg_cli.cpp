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

#define MODE_CONFIG 1
#define MODE_FRIENDLY 2
#define MODE_BOTH 3

bool get_blocks(CCfgConfig* config, const std::string& what, int mode) {
	XMLNode node;
	std::string name, desc;
	
	try {
		node = config->RootEx()->GoEx(what)->Child();
		while(node != NULL) {
			name = node->name();
			desc = node->first_attribute("description")->value();
			switch(mode) {
				case MODE_CONFIG:
					printf("%s\n", name.c_str());
					break;
				case MODE_FRIENDLY:
					printf("\"%s\"\n", desc.c_str());
					break;
				case MODE_BOTH:
					printf("%-10.10s \"%s\"\n", name.c_str(), desc.c_str());
					break;
			}
			node = node->next_sibling();
		}
	} catch(XMLException e) { 
		cout << e << endl;
		return false;
	}
	return true;
}

bool get_tasksets(CCfgConfig* config, const std::string& what, 
		const std::string& block, int mode) {
	XMLNode node;
	XMLAttr key;
	std::string name, desc;
	
	try {
		node = config->RootEx()->GoEx(what)->GoEx(block)->
			GoEx("taskset")->Leaf();
		while(node != NULL) {
			key = node->first_attribute("key");
			if(key == NULL)
				break;
			name = key->value();

			desc = config->RootEx()->GoEx("taskset")->GoEx(name)->
				GetAttrEx("description");

			switch(mode) {
				case MODE_CONFIG:
					printf("%s\n", name.c_str());
					break;
				case MODE_FRIENDLY:
					printf("\"%s\"\n", desc.c_str());
					break;
				case MODE_BOTH:
					printf("%-10.10s \"%s\"\n", name.c_str(), desc.c_str());
					break;
			}
			node = node->next_sibling("taskset");
		}
	} catch(XMLException e) { 
		cout << e << endl;
		return false;
	}
	return true;
}

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

	get_blocks(config, "online", MODE_BOTH);
	get_tasksets(config, "online", "mi", MODE_BOTH);
	
	return 0;
	
	


	return true;
}
