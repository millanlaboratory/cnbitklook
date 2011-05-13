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
#define GET_FILENAME	4	
#define GET_LOGLINE		5
#define GET_LOGFILENAME	6

using namespace std;

void get_daystamp(std::string* timestamp) {
	char temp[128];

	struct tm *tmp = NULL; 
	time_t t;

	t = time(NULL);
	tmp = localtime(&t);
	sprintf(temp, "%04d%02d%02d.%02d%02d%02d", 
			tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, 
			tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	timestamp->clear();
	timestamp->assign(temp);
}

void get_datestamp(std::string* timestamp) {
	char temp[128];

	struct tm *tmp = NULL; 
	time_t t;

	t = time(NULL);
	tmp = localtime(&t);
	sprintf(temp, "%04d%02d%02d", 
			tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday);

	timestamp->clear();
	timestamp->assign(temp);
}

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
			key = node->first_attribute("ttype");
			if(key == NULL)
				break;
			name = key->value();

			desc = config->RootEx()->GoEx("tasksets")->GoEx(name)->
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
		const std::string& block, const std::string& taskset, int mode) {
	CCfgTaskset* ts = NULL;
	try {
		if(what.find("online") != std::string::npos) {
			ts = config->OnlineEx(block, taskset);
		} else {
			fprintf(stderr, "Error: cannot get classifier when modality is offline\n");
			return false;
		}
	} catch(XMLException e) { 
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 0;
	}
	
	switch(mode) {
		case MODE_CONFIG:
			printf("%s\n", ts->classifier.filename.c_str());
			break;
		case MODE_FRIENDLY:
			printf("\"%s\"\n", ts->classifier.name.c_str());
			break;
		case MODE_BOTH:
			printf("%-30.30s \"%s\"\n", ts->classifier.filename.c_str(), 
					ts->classifier.name.c_str());
			break;
	}
	return true;
}

bool get_filename(CCfgConfig* config, const std::string& what, 
		const std::string& block, const std::string& taskset) {
	
	std::string subjectid = 
		config->RootEx()->QuickStringEx("subject/id");
	std::string daystamp;
	get_daystamp(&daystamp);

	printf("%s.%s.%s.%s.%s\n",
			subjectid.c_str(), daystamp.c_str(),
			what.c_str(), block.c_str(), taskset.c_str());

	return true;
}

bool get_logline(CCfgConfig* config, const std::string& what, 
		const std::string& block, const std::string& taskset) {
	std::string subjectid = 
		config->RootEx()->QuickStringEx("subject/id");
	std::string experimenter = 
		config->RootEx()->QuickStringEx("recording/experimenter");
	
	std::string daystamp;
	get_daystamp(&daystamp);

	printf("\"Experimenter=%s Subject=%s Daystamp=%s Modality=%s Block=%s Taskset=%s\"\n",
			experimenter.c_str(), subjectid.c_str(), daystamp.c_str(), 
			what.c_str(), block.c_str(), taskset.c_str());

	return true;
}

bool get_logfilename(CCfgConfig* config) {
	std::string subjectid = 
		config->RootEx()->QuickStringEx("subject/id");
	
	std::string datestamp;
	get_datestamp(&datestamp);

	printf("%s.%s.log\n", subjectid.c_str(), datestamp.c_str());

	return true;
}

void usage(void) { 
	printf("Usage: ccfg_cli [OPTION]...\n\n");
	printf("  -x FILE     XML file\n");
	printf("  -b          get available blocks (default)\n");
	printf("  -t BLOCK    get available tasksets for block BLOCK\n");
	printf("  -c          get the classifier for BLOCK/TASKSET\n");
	printf("  -F          get the basename for an XDF file\n");
	printf("  -L          get the filename for the XDF file log\n");
	printf("  -l          get the logline for an XDF file\n");
	printf("  -n          online modality (default)\n");
	printf("  -f          offline modality\n");
	printf("  -m MODALITY 'online' (default) or 'offline'\n");
	printf("  -u          enable user-friendly output\n");
	printf("  -U          user-friendly output only\n");
	printf("  -B BLOCK    set block\n");
	printf("  -T TASKSET  set taskset\n");
	printf("  -h          display this help and exit\n");
}

int main(int argc, char *argv[]) {
	int opt;	
	std::string filename, what("online"), block, taskset;
	int mode = MODE_CONFIG, get = GET_BLOCKS;

	if(argc == 1) {
		usage();
		return 0;
	}

	while((opt = getopt(argc, argv, "T:B:m:x:t:bnfuUhcFLl")) != -1) {
		if(opt == 'x')
			filename.assign(optarg);
		else if(opt == 'b')
			get = GET_BLOCKS;
		else if(opt == 't') {
			get = GET_TASKS;
			block.assign(optarg);
		}
		else if(opt == 'F') 
			get = GET_FILENAME;
		else if(opt == 'L') 
			get = GET_LOGFILENAME;
		else if(opt == 'l') 
			get = GET_LOGLINE;
		else if(opt == 'c') 
			get = GET_CLASSIFIER;
		else if(opt == 'n')
			what.assign("online");
		else if(opt == 'f')
			what.assign("offline");
		else if(opt == 'm')
			what.assign(optarg);
		else if(opt == 'u')
			mode = MODE_BOTH;
		else if(opt == 'B')
			block.assign(optarg);
		else if(opt == 'T')
			taskset.assign(optarg);
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
			return get_classifier(config, what, block, taskset, mode);
		case GET_FILENAME:
			return get_filename(config, what, block, taskset);
		case GET_LOGFILENAME:
			return get_logfilename(config);
		case GET_LOGLINE:
			return get_logline(config, what, block, taskset);
		default:
			return 666;
	}
	return 0;
}
