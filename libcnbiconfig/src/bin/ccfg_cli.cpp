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
#include <stdlib.h>
#include <unistd.h>

#define WHAT_ONLINE 	1
#define WHAT_OFFLINE 	2
#define MODE_CONFIG 	1
#define MODE_FRIENDLY 	2
#define MODE_BOTH		3
#define GET_BLOCKS 		1
#define GET_TASKS  		2
#define GET_CLASSIFIER	3

using namespace std;

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
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 0;
	}
	return 1;
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
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 0;
	}
	return 1;
}

bool get_classifier(CCfgConfig* config, const std::string& what, 
		const std::string& block) {


	return true;
}

void usage(void) { 
	printf("Usage: ccfg_cli [OPTION]...\n\n");
	printf("  -x FILE     XML file\n");
	printf("  -b          get available blocks (default)\n");
	printf("  -t BLOCK    get available tasksets for block BLOCK\n");
	printf("  -c BLOCK    get the classifier for block BLOCK\n");
	printf("  -n          online modality (default)\n");
	printf("  -f          offline modality\n");
	printf("  -m MODALITY 'online' (default) or 'offline'\n");
	printf("  -u          enable user-friendly output\n");
	printf("  -U          user-friendly output only\n");
	printf("  -h          display this help and exit\n");
}

int main(int argc, char *argv[]) {
	int opt;	
	std::string filename, what("online"), block;
	int mode = MODE_CONFIG, get = GET_BLOCKS;

	if(argc == 1) {
		usage();
		return 0;
	}

	while((opt = getopt(argc, argv, "c:m:x:bt:nfuUh")) != -1) {
		if(opt == 'x')
			filename.assign(optarg);
		else if(opt == 'b')
			get = GET_BLOCKS;
		else if(opt == 't') {
			get = GET_TASKS;
			block.assign(optarg);
		}
		else if(opt == 'c') {
			get = GET_CLASSIFIER;
			block.assign(optarg);
		}
		else if(opt == 'n')
			what.assign("online");
		else if(opt == 'f')
			what.assign("offline");
		else if(opt == 'm')
			what.assign(optarg);
		else if(opt == 'u')
			mode = MODE_BOTH;
		else if(opt == 'U')
			mode = MODE_FRIENDLY;
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	CCfgConfig* config;
	try {
		config = new CCfgConfig();
		config->ImportFileEx(filename);
	} catch(XMLException e) {
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return -1;
	}
	
	switch(get) {
		case GET_BLOCKS:
			return get_blocks(config, what, mode);
		case GET_TASKS:
			return get_tasksets(config, what, block, mode);
		case GET_CLASSIFIER:
			return get_classifier(config, what, block);
		default:
			return 666;
	}
	return 0;
}
