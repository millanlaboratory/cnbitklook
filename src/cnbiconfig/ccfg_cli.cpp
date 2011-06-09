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

#define MODALITY_NOTHING	0
#define MODALITY_ONLINE 	1
#define MODALITY_OFFLINE 	2

#define MODE_CONFIG 	1
#define MODE_FRIENDLY 	2
#define MODE_BOTH		3

#define GET_NOTHING		0
#define GET_BLOCKS 		1
#define GET_TASKSETS	2
#define GET_CLASSIFIER	3
#define GET_XDFBNAME	4	
#define GET_LOGLINE		5
#define GET_LOGFILENAME	6
#define GET_PROTOCOL	7

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

bool get_blocks(CCfgConfig* config, const std::string& modality, int mode) {
	XMLNode node;
	std::string name, desc;
	
	try {
		node = config->RootEx()->GoEx(modality)->Child();
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

bool get_protocol(CCfgConfig* config, const std::string& modality, 
		const std::string& block, const std::string& taskset) {
	std::string protocol;
	
	try {
		protocol = config->RootEx()->GoEx(modality)->GoEx(block)->GetAttrEx("protocol");
	} catch(XMLException e) { 
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 0;
	}
	
	printf("%s\n", protocol.c_str());
	return 1;
}

bool get_tasksets(CCfgConfig* config, const std::string& modality, 
		const std::string& block, int mode) {
	XMLNode node;
	XMLAttr key;
	std::string name, desc;
	
	try {
		node = config->RootEx()->GoEx(modality)->GoEx(block)->
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

bool get_classifier(CCfgConfig* config, const std::string& modality, 
		const std::string& block, const std::string& taskset, int mode) {
	CCfgTaskset* ts = NULL;
	try {
		if(modality.find("online") != std::string::npos) {
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

bool get_filename(CCfgConfig* config, const std::string& modality, 
		const std::string& block, const std::string& taskset) {
	
	std::string subjectid = 
		config->RootEx()->QuickStringEx("subject/id");
	std::string daystamp;
	get_daystamp(&daystamp);

	printf("%s.%s.%s.%s.%s\n",
			subjectid.c_str(), daystamp.c_str(),
			modality.c_str(), block.c_str(), taskset.c_str());

	return true;
}

bool get_logline(CCfgConfig* config, const std::string& modality, 
		const std::string& block, const std::string& taskset) {
	std::string subjectid = 
		config->RootEx()->QuickStringEx("subject/id");
	std::string experimenter = 
		config->RootEx()->QuickStringEx("recording/experimenter");
	
	std::string daystamp;
	get_daystamp(&daystamp);

	printf("\"Experimenter=%s Subject=%s Daystamp=%s Modality=%s Block=%s Taskset=%s\"\n",
			experimenter.c_str(), subjectid.c_str(), daystamp.c_str(), 
			modality.c_str(), block.c_str(), taskset.c_str());

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
	printf("  -F          set offline modality (equivalent to: '-M offline')\n");
	printf("  -N          set online modality (equivalent to: '-M online')\n");
	printf("  -M MODALITY set modality 'online' or 'offline'\n");
	printf("  -B BLOCK    set block\n");
	printf("  -T TASKSET  set taskset\n");
	printf("  -b          get available blocks (default)\n");
	printf("  -t BLOCK    get available tasksets for block BLOCK\n");
	printf("  -c          get the classifier for BLOCK/TASKSET\n");
	printf("  -a          get the basename for an XDF file\n");
	printf("  -o          get the xml for the XDF file log\n");
	printf("  -l          get the logline for an XDF file\n");
	printf("  -p          get the protocol executable name\n");
	printf("  -u          enable user-friendly output\n");
	printf("  -U          enable user-friendly output only\n");
	printf("  -h          display this help and exit\n\n");
	printf("Typical usage:\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Fbu\n");
	printf("    Get all the available offline modalities\n\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Nbu\n");
	printf("    Get all the available online modalities\n\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Nu -t errp\n");
	printf("    Get all the available online tasksets for the 'errp' block\n\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Nu -t errp -T errp\n");
	printf("    Get all the available online tasksets for the 'errp' block and\n");
	printf("    the 'errp' taskset in the online modality\n\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Nu -t errp -T errp -a\n");
	printf("    Get the XDF basename for the 'errp' block and\n");
	printf("    the 'errp' taskset in the online modality\n\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Nu -t errp -T errp -o\n");
	printf("    Get the XDF log xml for the 'errp' block and\n");
	printf("    the 'errp' taskset in the online modality\n\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Nu -t errp -T errp -l\n");
	printf("    Get the XDF log line for the 'errp' block and\n");
	printf("    the 'errp' taskset in the online modality\n\n");
	printf("  ccfg_cli -x $HOME/Desktop/example.xml -Nu -B errp -p\n");
	printf("    Get the protocol executable for the 'errp' block and\n");
	printf("    the 'errp' taskset in the online modality\n\n");
	printf("Keep in mind that:\n");
	printf("  1. You can play around with -u and -U. In this way you can print\n");
	printf("     both the XML keys and the XML descriptions (i.e. 'errp' and\n");
	printf("     'Error Potentials').\n");
	printf("  2. Certain commands do not support the -u and -U option. When not\n");
	printf("     supported, -u and -U will have no effect.\n");
	printf("  3. I have to work a bit on my PhD apart from coding your loops.\n");
	printf("     So, if you feel lost with all these options, stop complaining\n");
	printf("     and start coding. If you cannot code, bake me an apple cake\n");
	printf("     and I will do it for you.\n\n");
}

int main(int argc, char *argv[]) {
	int opt;	
	std::string xml, modality, block, taskset;
	int mode = MODE_CONFIG, get = GET_NOTHING;

	if(argc == 1) {
		usage();
		return 0;
	}

	while((opt = getopt(argc, argv, "T:B:M:x:t:bFNpuUhcaol")) != -1) {
		if(opt == 'x')
			xml.assign(optarg);
		else if(opt == 'b')
			get = GET_BLOCKS;
		else if(opt == 't') {
			get = GET_TASKSETS;
			block.assign(optarg);
		}
		else if(opt == 'a') 
			get = GET_XDFBNAME;
		else if(opt == 'o') 
			get = GET_LOGFILENAME;
		else if(opt == 'l') 
			get = GET_LOGLINE;
		else if(opt == 'c') 
			get = GET_CLASSIFIER;
		else if(opt == 'p') 
			get = GET_PROTOCOL;
		else if(opt == 'N')
			modality.assign("online");
		else if(opt == 'F')
			modality.assign("offline");
		else if(opt == 'M')
			modality.assign(optarg);
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
	
	bool die = false;
	if(xml.empty()) {
		fprintf(stderr, "Error: XML configuration required\n");
		die = true;
	}
	if(modality.empty()) {
		fprintf(stderr, "Error: modality unknown (online, offline)\n");
		die = true;
	}
	if(get == GET_NOTHING) {
		fprintf(stderr, "Error: define something to get (i.e. block, tasksets...)\n");
		die = true;
	}

	if(die == true)
		return 1;

	CCfgConfig* config;
	try {
		config = new CCfgConfig();
		config->ImportFileEx(xml);
	} catch(XMLException e) {
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 2;
	}
	
	switch(get) {
		case GET_BLOCKS:
			return get_blocks(config, modality, mode);
		case GET_TASKSETS:
			return get_tasksets(config, modality, block, mode);
		case GET_PROTOCOL:
			return get_protocol(config, modality, block, taskset);
		case GET_CLASSIFIER:
			return get_classifier(config, modality, block, taskset, mode);
		case GET_XDFBNAME:
			return get_filename(config, modality, block, taskset);
		case GET_LOGFILENAME:
			return get_logfilename(config);
		case GET_LOGLINE:
			return get_logline(config, modality, block, taskset);
		default:
			return 666;
	}
	return 0;
}
