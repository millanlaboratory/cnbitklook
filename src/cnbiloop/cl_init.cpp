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

#define WHAT_NOTHING		0
#define WHAT_START			1
#define WHAT_STOP			2
#define WHAT_MONITOR		3

#define MODALITY_NOTHING	0
#define MODALITY_ONLINE		1
#define MODALITY_OFFLINE 	2

#define MODULE_NOTHING		0
#define MODULE_CLASSIFIER	1
#define MODULE_PROTOCOL		2

#include "ClLoop.hpp"
#include <cnbicore/CcBasic.hpp>

bool classifier_start(CCfgConfig* config, const std::string& block, 
		const std::string& taskset) {
	CCfgTaskset* ts = NULL;
	try {
		ts = config->OnlineEx(block, taskset);
	} catch(XMLException e) { 
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 0;
	}

	int pid = 0;
	if(ClLoop::processing.ForkAndCheck(&pid) != ClProLang::Successful) {
		fprintf(stderr, "Error: cannot fork\n");
		return 0;
	}
	
	ClLoop::processing.Directory(pid, ts->ndf.directory);
	ClLoop::processing.IncludeNDF(pid);
	if(ts->ndf.include.empty() == false)
		ClLoop::processing.Include(pid, ts->ndf.include);
	ClLoop::processing.Exec(pid, ts->ndf.exec);

	return(ClLoop::processing.IsAlive(pid) == ClProLang::Successful);
}

bool classifier_monitor(CCfgConfig* config, const std::string& block, 
		const std::string& taskset) {
	return 1;
}

void usage(void) { 
	printf("Usage: cl_init [OPTION]...\n\n");
	printf("  -x FILE     XML file\n");
	printf("  -F          set offline modality\n");
	printf("  -N          set online modality\n");
	printf("  -B BLOCK    set block\n");
	printf("  -T TASKSET  set taskset\n");
	printf("  -s          start\n");
	printf("  -t          stop\n");
	printf("  -m          monitor\n");
	printf("  -c          classifier (can only be started or monitored, sets -N)\n\n");
	printf("Typical usage:\n");
	printf("  cl_init -x example.xml -sc -N -B mi -T mi_rhlh\n");
	printf("    Start the classifier for block 'mi' and taskset 'mi_rhlh'\n");
	printf("  cl_init -x example.xml -mc -N -B mi -T mi_rhlh\n");
	printf("    Monitor the classifier for block 'mi' and taskset 'mi_rhlh'\n");
}

int main(int argc, char* argv[]) {
	int opt, modality = MODALITY_NOTHING, what = WHAT_NOTHING, module = MODULE_NOTHING;
	std::string xml, block, taskset;
	if(argc < 2) {
		usage();
		CcCore::Exit(0);
	}

	CcCore::OpenLogger("cl_init", CcCore::TerminalDisabled);
	
	while((opt = getopt(argc, argv, "x:M:B:T:FNstmc")) != -1) {
		if(opt == 'x')
			xml.assign(optarg);
		else if(opt == 'F')
			modality = MODALITY_OFFLINE;
		else if(opt == 'N')
			modality = MODALITY_ONLINE;
		else if(opt == 'B')
			block.assign(optarg);
		else if(opt == 'T')
			taskset.assign(optarg);
		else if(opt == 's')
			what = WHAT_START;
		else if(opt == 't')
			what = WHAT_STOP;
		else if(opt == 'm')
			what = WHAT_MONITOR;
		else if(opt == 'c') {
			module = MODULE_CLASSIFIER;
			modality = MODALITY_ONLINE;
		} else if(opt == 'p')
			what = MODULE_PROTOCOL;
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}
	
	bool die = false;
	if(xml.empty()) {
		fprintf(stderr, "Error: XML configuration required\n");
		die = true;
	}
	if(modality == MODALITY_NOTHING) {
		fprintf(stderr, "Error: modality unknown\n");
		die = true;
	}
	if(block.empty()) {
		fprintf(stderr, "Error: block unknown\n");
		die = true;
	}
	if(taskset.empty()) {
		fprintf(stderr, "Error: taskset unknown\n");
		die = true;
	}
	if(what == WHAT_NOTHING) {
		fprintf(stderr, "Error: define an action\n");
		die = true;
	}
	if(module == MODULE_NOTHING) {
		fprintf(stderr, "Error: define a module\n");
		die = true;
	}
	
	if(die == true)
		CcCore::Exit(1);;
	
	CCfgConfig* config = NULL;
	try {
		config = new CCfgConfig();
		config->ImportFileEx(xml);
	} catch(XMLException e) {
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		CcCore::Exit(2);;
	}

	if(ClLoop::Connect() == false) {
		fprintf(stderr, "Error: cannot connect to loop\n");
		CcCore::Exit(3);;
	}

	bool status = false;
	if(module == MODULE_CLASSIFIER) { 
		if(what == WHAT_START) 
			status = classifier_start(config, block, taskset);
		else if(what == WHAT_MONITOR)
			status = classifier_monitor(config, block, taskset);
		else {
			fprintf(stderr, "Error: classifier cannot be stopped\n");
			status = false;
		}
	}
	CcCore::Exit(status);;
}
