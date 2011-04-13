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

void connect(ClLoop* loop) {
	if(loop->Connect() == false) {
		printf("Error: cannot connect to loop\n");
		exit(1);
	}
}

void processing(int status) { 
	switch(status) {
		case ClProLang::StatusLost:
			fprintf(stderr, "StatusLost\n");
			break;
		case ClProLang::NotSupported:
			fprintf(stderr, "NotSupported\n");
			break;
		case ClProLang::NotUndestood:
			fprintf(stderr, "NotUndestood\n");
			break;
		case ClProLang::NoReply:
			fprintf(stderr, "NoReply\n");
			break;
		case ClProLang::Successful:
			fprintf(stderr, "Successful\n");
			break;
		case ClProLang::ForkFailed:
			fprintf(stderr, "ForkFailed\n");
			break;
		case ClProLang::IsDead:
			fprintf(stderr, "IsDead\n");
			break;
		case ClProLang::NotFound:
			fprintf(stderr, "NotFound\n");
			break;
	}
}

void acquisition(int status) { 
	switch(status) {
		case ClAcqLang::StatusLost:
			fprintf(stderr, "StatusLost\n");
			break;
		case ClAcqLang::NotSupported:
			fprintf(stderr, "NotSupported\n");
			break;
		case ClAcqLang::NotUndestood:
			fprintf(stderr, "NotUndestood\n");
			break;
		case ClAcqLang::NoReply:
			fprintf(stderr, "NoReply\n");
			break;
		case ClAcqLang::Successful:
			fprintf(stderr, "Successful\n");
			break;
		case ClAcqLang::NDFLimitReached:
			fprintf(stderr, "NDFLimitReached\n");
			break;
		case ClAcqLang::XDFAlreadyOpen:
			fprintf(stderr, "XDFAlreadyOpen\n");
			break;
		case ClAcqLang::XDFOpenFailed:
			fprintf(stderr, "XDFOpenFailed\n");
			break;
		case ClAcqLang::LogOpenFailed:
			fprintf(stderr, "LogOpenFailed\n");
			break;
		case ClAcqLang::XDFNotOpen:
			fprintf(stderr, "XDFNotOpen\n");
			break;
		case ClAcqLang::XDFCloseFailed:
			fprintf(stderr, "XDFCloseFailed\n");
			break;
		case ClAcqLang::XDFSetupFailed:
			fprintf(stderr, "XDFSetupFailed\n");
			break;
	}
}

void nameserver(int status) { 
	switch(status) {
		case ClNamesLang::StatusLost:
			fprintf(stderr, "StatusLost\n");
			break;
		case ClNamesLang::NotSupported:
			fprintf(stderr, "NotSupported\n");
			break;
		case ClNamesLang::NotUndestood:
			fprintf(stderr, "NotUndestood\n");
			break;
		case ClNamesLang::NoReply:
			fprintf(stderr, "NoReply\n");
			break;
		case ClNamesLang::Successful:
			fprintf(stderr, "Successful\n");
			break;
		case ClNamesLang::NotFound:
			fprintf(stderr, "NotFound\n");
			break;
		case ClNamesLang::AlreadySet:
			fprintf(stderr, "AlreadySet\n");
			break;
		case ClNamesLang::AlreadyStored:
			fprintf(stderr, "AlreadyStored\n");
			break;
		case ClNamesLang::NotAvailable:
			fprintf(stderr, "NotAvailable\n");
			break;
		case ClNamesLang::NameFormatError:
			fprintf(stderr, "NameFormatError\n");
			break;
	}
}

int main(int argc, char* argv[]) {
	if(argc < 2)
		usage();

	int status = 1;;
	ClLoop cl;
	std::string command(argv[1]), arg1, arg2, arg3;

	if(argc >= 3) arg1.assign(argv[2]);
	if(argc >= 4) arg2.assign(argv[3]);
	if(argc >= 5) arg3.assign(argv[4]);
	
	if(command.compare("help") == 0) {
	} else if(command.compare("openxdf") == 0) {
		if(argc != 5) usage();
		connect(&cl);
	
		status = cl.acquisition.OpenXDF(arg1, arg2, arg3);
		acquisition(status);
		return(status);

	} else if(command.compare("closexdf") == 0) {
		connect(&cl);

		status = cl.acquisition.CloseXDF();
		acquisition(status);
		return(status);

	} else if(command.compare("set") == 0) {
		if(argc != 4) usage();
		connect(&cl);

		status = cl.nameserver.Set(arg1, arg2);
		nameserver(status);
		return(status);

	} else if(command.compare("unset") == 0) {
		if(argc != 3) usage();
		connect(&cl);

		status = cl.nameserver.Unset(arg1);
		return(status);

	} else if(command.compare("query") == 0) {
		if(argc != 3) usage();
		connect(&cl);

		CcAddress address;
		status = cl.nameserver.Query(arg1, &address);
		nameserver(status);
		if(status == ClNamesLang::Successful)
			cout << address << endl;
		return(status);

	} else if(command.compare("store") == 0) {
		if(argc != 4) usage();
		connect(&cl);

		status = cl.nameserver.Store(arg1, arg2);
		nameserver(status);
		return(status);

	} else if(command.compare("retrieve") == 0) {
		if(argc != 3) usage();
		connect(&cl);

		std::string storage;
		status = cl.nameserver.Retrieve(arg1, &storage);
		nameserver(status);
		if(status == ClNamesLang::Successful)
			cout << storage << endl;

	} else if(command.compare("erase") == 0) {
		if(argc != 3) usage();
		connect(&cl);

		status = cl.nameserver.Erase(arg1);
		nameserver(status);
		return(status);

	} else if(command.compare("fork") == 0) {
		connect(&cl);

		int pid;
		status = cl.processing.ForkAndCheck(&pid);
		processing(status);
		if(status == ClProLang::Successful)
			cout << pid << endl;
		return(status);

	} else if(command.compare("launch") == 0) {
		if(argc != 4) usage();
		connect(&cl);
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		processing(status);
		status = cl.processing.Launch(pid, arg2);
		return(status);

	} else if(command.compare("isalive") == 0) {
		if(argc != 3) usage();
		connect(&cl);
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = cl.processing.IsAlive(pid);
		processing(status);
		if(status == ClProLang::Successful) 
			cout << "alive" << endl;
		else if(status == ClProLang::IsDead)
			cout << "dead" << endl;
		return(status);

	} else if(command.compare("terminate") == 0) {
		if(argc != 3) usage();
		connect(&cl);
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = cl.processing.Terminate(pid);
		processing(status);
		return(status);

	} else if(command.compare("cwd") == 0) {
		if(argc != 4) usage();
		connect(&cl);

		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = cl.processing.ChangeDirectory(pid, arg2);
		processing(status);
		return(status);
	
	} else if(command.compare("include") == 0) {
		if(argc != 5) usage();
		connect(&cl);
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = cl.processing.Include(pid, arg1, arg2);
		processing(status);
		return(status);

	} else {
		usage();
	}
	cl.Disconnect();
	return 0;
}
