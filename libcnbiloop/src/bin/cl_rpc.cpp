/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ClLoop.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

void usage(void) { 
	printf("Usage: cl_rpc [-p]\n");
	printf("Where: -p      true/false\n");
	exit(1);
}

int main(int argc, char* argv[]) {
	if(argc < 2)
		usage();
	
	/*
	CcCore::OpenLogger("clallclients", CcCore::LevelWarning);
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	*/

	ClLoop cl;
	if(cl.Connect() == false) {
		printf("Error: cannot connect to loop\n");
		exit(1);
	}
	
	cout << argv[1] << endl;
	std::string command(argv[1]);
	if(command.compare("help") == 0) {
	} else if(command.compare("openxdf") == 0) {
		if(argc != 5) usage();
	} else if(command.compare("closexdf") == 0) {
	} else if(command.compare("set") == 0) {
		if(argc != 4) usage();
	} else if(command.compare("unset") == 0) {
		if(argc != 3) usage();
	} else if(command.compare("query") == 0) {
		if(argc != 3) usage();
	} else if(command.compare("store") == 0) {
		if(argc != 4) usage();
	} else if(command.compare("retrieve") == 0) {
		if(argc != 3) usage();
	} else if(command.compare("erase") == 0) {
		if(argc != 3) usage();
	} else if(command.compare("fork") == 0) {
	} else if(command.compare("launch") == 0) {
		if(argc != 4) usage();
	} else if(command.compare("isalive") == 0) {
		if(argc != 3) usage();
	} else if(command.compare("terminate") == 0) {
		if(argc != 3) usage();
	} else if(command.compare("cwd") == 0) {
		if(argc != 4) usage();
	} else if(command.compare("include") == 0) {
		if(argc != 5) usage();
	} else {
		usage();
	}

shutdown:
	return 0;
}
