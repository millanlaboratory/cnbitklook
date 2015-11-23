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
#define WHAT_LOAD			4
#define WHAT_UNLOAD			5
#define WHAT_DUMP			6

#define MODALITY_NOTHING	0
#define MODALITY_ONLINE		1
#define MODALITY_OFFLINE 	2

#define MODULE_NOTHING		0
#define MODULE_CLASSIFIER	1
#define MODULE_PROTOCOL		2
#define MODULE_LOOP			3

#include "ClLoop.hpp"
#include <cnbicore/CcBasic.hpp>

bool dump_configuration(CCfgConfig* config, const int modality,
		const std::string& block, const std::string& taskset) {
	CCfgTaskset* ts = NULL;
	try {
		if(modality == MODALITY_ONLINE) {
			ts = config->OnlineEx(block, taskset);
		} else {
			ts = config->OfflineEx(block, taskset);
		}
	} catch(XMLException e) { 
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 0;
	}

	ts->Dump();
	ts->DumpNDF();
	ts->DumpClassifier();

	return 1;
}

int unload_configuration(const std::string& block) {
	bool status[5];

	status[0] = ClLoop::nms.EraseConfig(block, "block");
	status[1] = ClLoop::nms.EraseConfig(block, "taskset");
	status[2] = ClLoop::nms.EraseConfig(block, "xml");
	status[3] = ClLoop::nms.EraseConfig(block, "path");
	status[4] = ClLoop::nms.EraseConfig(block, "modality");
	
	for(int i = 0; i < 5; i++)
		if(status[i] != true)
			return 1;
	return 0;
}

int load_configuration(CCfgConfig* config, std::string file, const int modality,
		const std::string& block, const std::string& taskset) {
	CCfgTaskset* ts = NULL;
	try {
		if(modality == MODALITY_ONLINE) {
			ts = config->OnlineEx(block, taskset);
		} else {
			ts = config->OfflineEx(block, taskset);
		}
	} catch(XMLException e) { 
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 1;
	}

	std::string component = block;
	
	std::string path;
	size_t pos = file.rfind("/");
	if(pos == std::string::npos) {
		fprintf(stderr, "Error: XML path (%s) not absolute or something wrong\n",
				file.c_str());
		return 2;
	} else {
		path = file.substr(0, pos+1);
	}

	bool status[5];
	status[0] = ClLoop::nms.StoreConfig(component, "modality", 
			modality == MODALITY_ONLINE ?  "online" : "offline");
	status[1] = ClLoop::nms.StoreConfig(component, "block", block);
	status[2] = ClLoop::nms.StoreConfig(component, "taskset", ts->name);
	status[3] = ClLoop::nms.StoreConfig(component, "xml", file);
	status[4] = ClLoop::nms.StoreConfig(component, "path", path);

	for(int i = 0; i < 5; i++) {
		if(status[i] != true)
			return 3;
	}
	return 0;
}

int classifier_start(CCfgConfig* config, const std::string& block, 
		const std::string& taskset, bool monitor) {
	CCfgTaskset* ts = NULL;
	try {
		ts = config->OnlineEx(block, taskset);
	} catch(XMLException e) { 
		if(monitor == false)
			printf("%d\n", 0);
		fprintf(stderr, "Error: %s\n", e.Info().c_str());
		return 0;
	}

	int pid = 0;
	if(ClLoop::pro.ForkAndCheck(&pid) != ClProLang::Successful) {
		printf("%d\n", 0);
		fprintf(stderr, "Error: cannot fork\n");
		return 0;
	}
	ClLoop::pro.Directory(pid, ts->ndf.directory);
	ClLoop::pro.IncludeNDF(pid);
	if(ts->ndf.include.empty() == false)
		ClLoop::pro.Include(pid, ts->ndf.include);
	ClLoop::pro.Exec(pid, ts->ndf.exec);

	if(monitor == true) {
		CcCore::CatchSIGINT();
		CcCore::CatchSIGTERM();
		CcTimeValue tic;
		CcTime::Tic(&tic);
		printf("[cl_init] Monitoring classifier with PID=%d (SIGTERM/SIGINT to stop)\n", 
				pid); 
		while(true) {
			if(CcCore::receivedSIGAny.Get()) {
				printf("[cl_init] Terminating classifier with PID=%d\n", pid);
				return(ClLoop::pro.Terminate(pid) == ClProLang::Successful);
			}

			if(ClLoop::pro.IsAlive(pid) != ClProLang::Successful) {
				printf(" Classifier '%s' for block '%s' PID=%d died (%.2f minutes)\n", 
						ts->classifier.id.c_str(), block.c_str(), 
						pid, CcTime::Toc(&tic)/1000/60);
				return 0;
			}
			printf(" Classifier '%s' for block '%s' PID=%d running (%.2f minutes)\n", 
					ts->classifier.id.c_str(), block.c_str(), 
					pid, CcTime::Toc(&tic)/1000/60);
			CcTime::Sleep(5000);
		}
	} else {
		if(ClLoop::pro.IsAlive(pid) != ClProLang::Successful) {
			printf("%d\n", 0);
			return 0;
		} else {
			printf("%d\n", pid);
		}
	}

	return pid;
}

void usage(void) { 
	printf("Usage: cl_init [OPTION]...\n\n");
	printf("  -x FILE     XML file (absolute path when using -l)\n");
	printf("  -F          set offline modality\n");
	printf("  -N          set online modality\n");
	printf("  -B BLOCK    set block\n");
	printf("  -T TASKSET  set taskset\n");
	printf("  -s          start\n");
	printf("  -t          stop\n");
	printf("  -m          monitor\n");
	printf("  -l          load loop configuration\n");
	printf("  -d          dump loop configuration\n");
	printf("  -u          unload loop configuration\n");
	printf("  -c          classifier (can only be started or monitored, sets -N)\n\n");
	printf("Typical usage:\n");
	printf("  cl_init -x example.xml -sc -N -B mi -T mi_rhlh\n");
	printf("    Start the classifier for block 'mi' and taskset 'mi_rhlh'\n");
	printf("  cl_init -x example.xml -mc -N -B mi -T mi_rhlh\n");
	printf("    Monitor the classifier for block 'mi' and taskset 'mi_rhlh'\n");
	printf("  cl_init -x example.xml -mc -N -B mi -T mi_rhlh\n");
	printf("    Monitor the classifier for block 'mi' and taskset 'mi_rhlh'\n");
	printf("  cl_init -x /full/path/to/example.xml -d -N -B mi -T mi_rhlh\n");
	printf("    Dump online loop configuration for block 'mi' and taskset 'mi_rhlh'\n");
	printf("  cl_init -x /full/path/to/example.xml -l -F -B mi -T mi_rhlh\n");
	printf("    Load offline loop configuration for block 'mi' and taskset 'mi_rhlh'\n");
	printf("  cl_init -u -B mi\n");
	printf("    Unload loop configuration for block 'mi'\n\n");
	printf("Suggested usage for checkloop:\n");
	printf("  cl_init -x $HOME/Desktop/example.xml -dN -B checkloop -T dummy\n");
	printf("    Dump loop configuration for block 'checkloop' and taskset 'dummy'\n");
	printf("  cl_init -x $HOME/Desktop/example.xml -lN -B checkloop -T dummy\n");
	printf("    Load the loop configuration on the nameserver'\n");
	printf("  cl_init -x $HOME/Desktop/example.xml -scN -B checkloop -T dummy\n");
	printf("    Ask the processing server to start a classifier'\n");
	printf("  cl_init -x $HOME/Desktop/example.xml -mcN -B checkloop -T dummy\n");
	printf("    As before, but monitoring the classifier'\n");
	printf("  cl_init -x $HOME/Desktop/example.xml -u -B checkloop\n");
	printf("    Unload the loop configuration'\n");
}

int main(int argc, char* argv[]) {
	int opt, modality = MODALITY_NOTHING, what = WHAT_NOTHING, module = MODULE_NOTHING;
	std::string xml, block, taskset;
	if(argc < 2) {
		usage();
		CcCore::Exit(0);
	}

	CcCore::OpenLogger("cl_init", CcCore::TerminalDisabled);
	ClLoop::Configure();
	
	while((opt = getopt(argc, argv, "x:M:B:T:FNsutlumdc")) != -1) {
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
		else if(opt == 'l') {
			what = WHAT_LOAD;
			module = MODULE_LOOP;
		}
		else if(opt == 'u') {
			what = WHAT_UNLOAD;
			module = MODULE_LOOP;
			modality = MODALITY_ONLINE;
			xml = "none";
			taskset = "none";
		}
		else if(opt == 'd') {
			what = WHAT_DUMP;
			module = MODULE_LOOP;
		}
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
	if(what != WHAT_UNLOAD) {
		try {
			config = new CCfgConfig();
			config->ImportFileEx(xml);
		} catch(XMLException e) {
			fprintf(stderr, "Error: %s\n", e.Info().c_str());
			CcCore::Exit(2);;
		}
	}

	if(ClLoop::Connect() == false) {
		fprintf(stderr, "Error: cannot connect to loop\n");
		CcCore::Exit(3);;
	}

	int status = 1;
	if(module == MODULE_CLASSIFIER) { 
		if(what == WHAT_START) 
			status = classifier_start(config, block, taskset, false);
		else if(what == WHAT_MONITOR)
			status = classifier_start(config, block, taskset, true);
		else {
			fprintf(stderr, "Error: not implemented\n");
			status = 666;
		}
	} else if (module == MODULE_LOOP) {
		if(what == WHAT_LOAD)
			status = load_configuration(config, xml, modality, block, taskset);
		else if(what == WHAT_UNLOAD)
			status = unload_configuration(block);
		else if(what == WHAT_DUMP)
			status = dump_configuration(config, modality, block, taskset);
		else {
			fprintf(stderr, "Error: not implemented\n");
			status = 666;
		}
	}
	CcCore::CloseLogger();
	return status;
}
