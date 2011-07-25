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

#include "ClLoop.hpp"
#include <cnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

void example(void) { 
	printf("Usage: cl_rpc [COMMAND] [OPTION]..\n\n");
	printf("Valid commands are:\n");
	printf(" help\n");
	printf(" example\n");
	printf(" openxdf test.gdf test.log \"threshold=0.76 integration=0.96\"\n");
	printf(" closexdf\n");
	printf(" updatelog \"extra1=0.72 extra2=2.50\"\n");
	printf(" set /acquisition 127.0.0.1:9000\n");
	printf(" unset /acquisition\n");
	printf(" query /acquisition\n");
	printf(" store ndf_monitor::display true\n");
	printf(" retrieve ndf_monitor::display\n");
	printf(" storefile ndf_monitor::display /home/mtavella/display.txt\n");
	printf(" retrievefile ndf_monitor::display /home/mtavella/display.txt\n");
	printf(" erase ndf_monitor::display\n");
	printf(" fork\n");
	printf(" exec 1231 \"bench(1);\"\n");
	printf(" isalive 1231\n");
	printf(" terminate 1231\n");
	printf(" directory 1231 /home/mtavella/\n");
	printf(" include 1231 /home/mtavella/ /home/ptito/\n");
	printf(" sinclude 1231 /home/mtavella/ /home/ptito/\n");
}

void usage(void) { 
	printf("Usage: cl_rpc [COMMAND] [OPTION]..\n\n");
	printf("Valid commands are:\n");
	printf(" help                              display this help and exit\n");
	printf(" example                           display the examples and exit\n");
	printf(" openxdf XDFFILE LOGFILE LOGLINE   open XDFFILE and adds LOGLINE to LOGFILE\n");
	printf(" closexdf                          close the currently open XDFFILE\n");
	printf(" updatelog LOGLINE                 adds LOGLINE to the log\n");
	printf(" set NAME IP:PORT                  sets a NAME\n");
	printf(" unset NAME                        unsets a NAME\n");
	printf(" query NAME                        queries a NAME\n");
	printf(" store NAME CONTENT                stores CONTENT under NAME\n");
	printf(" retrieve NAME                     retrieves the CONTENT stored under NAME\n");
	printf(" erase NAME                        erases the CONTENT stored under NAME\n");
	printf(" storeconfig COMP NAME CONTENT     stores CONTENT under COMP::NAME\n");
	printf(" retrieveconfig COMP NAME          retrieves the CONTENT stored under COMP::NAME\n");
	printf(" eraseconfig COMP NAME             erases the CONTENT stored under COMP::NAME\n");
	printf(" storefile NAME PATH               stores file PATH under NAME\n");
	printf(" retrievefile NAME PATH            retrieves NAME and save it in PATH\n");
	printf(" fork                              fork a Matlab process\n");
	printf(" exec PID COMMAND                  exec a COMMAND in Matlab process PID\n");
	printf(" isalive PID                       checks if PID is alive\n");
	printf(" terminate PID                     terminate PID\n");
	printf(" directory PID PATH                changes working directory of PID\n");
	printf(" include PID PATH                  include PATH\n");
	printf(" sinclude PID PATH1 PATH2          include PATH1 or PATH2\n");
	CcCore::Exit(0);
}

void connect(void) {
	if(ClLoop::Connect() == false) {
		printf("Error: cannot connect to loop\n");
		CcCore::Exit(1);
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
		case ClAcqLang::LogUpdateFailed:
			fprintf(stderr, "LogUpdateFailed\n");
			break;
	}
}

void nameserver(int status) { 
	switch(status) {
		case ClNmsLang::ErrorGeneric:
			fprintf(stderr, "ErrorGeneric\n");
			break;
		case ClNmsLang::StatusLost:
			fprintf(stderr, "StatusLost\n");
			break;
		case ClNmsLang::NotSupported:
			fprintf(stderr, "NotSupported\n");
			break;
		case ClNmsLang::NotUndestood:
			fprintf(stderr, "NotUndestood\n");
			break;
		case ClNmsLang::NoReply:
			fprintf(stderr, "NoReply\n");
			break;
		case ClNmsLang::Successful:
			fprintf(stderr, "Successful\n");
			break;
		case ClNmsLang::NotFound:
			fprintf(stderr, "NotFound\n");
			break;
		case ClNmsLang::AlreadySet:
			fprintf(stderr, "AlreadySet\n");
			break;
		case ClNmsLang::AlreadyStored:
			fprintf(stderr, "AlreadyStored\n");
			break;
		case ClNmsLang::NotAvailable:
			fprintf(stderr, "NotAvailable\n");
			break;
		case ClNmsLang::NameFormatError:
			fprintf(stderr, "NameFormatError\n");
			break;
	}
}

int main(int argc, char* argv[]) {
	if(argc < 2)
		usage();
	
	CcCore::OpenLogger("cl_rpc", CcCore::TerminalDisabled);

	int status = 1;;
	std::string command(argv[1]), arg1, arg2, arg3;

	if(argc >= 3) arg1.assign(argv[2]);
	if(argc >= 4) arg2.assign(argv[3]);
	if(argc >= 5) arg3.assign(argv[4]);
	
	if(command.compare("help") == 0) {
		usage();
	} else if(command.compare("example") == 0) {
		example();
	} else if(command.compare("openxdf") == 0) {
		if(argc != 5) usage();
		connect();
	
		status = ClLoop::acq.OpenXDF(arg1, arg2, arg3);
		acquisition(status);
		CcCore::Exit(status);

	} else if(command.compare("closexdf") == 0) {
		connect();

		status = ClLoop::acq.CloseXDF();
		acquisition(status);
		CcCore::Exit(status);
	
	} else if(command.compare("updatelog") == 0) {
		if(argc != 3) usage();
		connect();

		status = ClLoop::acq.UpdateLog(arg1);
		acquisition(status);
		CcCore::Exit(status);

	} else if(command.compare("set") == 0) {
		if(argc != 4) usage();
		connect();

		status = ClLoop::nms.Set(arg1, arg2);
		nameserver(status);
		CcCore::Exit(status);

	} else if(command.compare("unset") == 0) {
		if(argc != 3) usage();
		connect();

		status = ClLoop::nms.Unset(arg1);
		CcCore::Exit(status);

	} else if(command.compare("query") == 0) {
		if(argc != 3) usage();
		connect();

		CcAddress address;
		status = ClLoop::nms.Query(arg1, &address);
		nameserver(status);
		if(status == ClNmsLang::Successful)
			cout << address << endl;
		CcCore::Exit(status);

	} else if(command.compare("store") == 0) {
		if(argc != 4) usage();
		connect();

		status = ClLoop::nms.Store(arg1, arg2);
		nameserver(status);
		CcCore::Exit(status);

	} else if(command.compare("retrieve") == 0) {
		if(argc != 3) usage();
		connect();

		std::string storage;
		status = ClLoop::nms.Retrieve(arg1, &storage);
		nameserver(status);
		if(status == ClNmsLang::Successful)
			cout << storage << endl;
	
	} else if(command.compare("erase") == 0) {
		if(argc != 3) usage();
		connect();
		
		status = ClLoop::nms.Erase(arg3);
		nameserver(status);
		CcCore::Exit(status);
	
	} else if(command.compare("storeconfig") == 0) {
		if(argc != 5) usage();
		connect();

		status = ClLoop::nms.StoreConfig(arg1, arg2, arg3);
		status = status == 0 ? 
			ClNmsLang::ErrorGeneric : ClNmsLang::Successful;
		nameserver(status);
		CcCore::Exit(status);

	} else if(command.compare("retrieveconfig") == 0) {
		if(argc != 4) usage();
		connect();

		std::string storage = ClLoop::nms.RetrieveConfig(arg1, arg2);
		status = storage.empty() == 1 ? 
			ClNmsLang::ErrorGeneric : ClNmsLang::Successful;
		nameserver(status);
		if(status == ClNmsLang::Successful)
			cout << storage << endl;
	
	} else if(command.compare("eraseconfig") == 0) {
		if(argc != 4) usage();
		connect();

		status = ClLoop::nms.EraseConfig(arg1, arg2);
		status = status == 0 ?
			ClNmsLang::ErrorGeneric : ClNmsLang::Successful;
		nameserver(status);
		CcCore::Exit(status);
	
	} else if(command.compare("storefile") == 0) {
		if(argc != 4) usage();
		connect();

		bool status = ClLoop::nms.StoreFile(arg1, arg2);
		CcCore::Exit(status);

	} else if(command.compare("retrievefile") == 0) {
		if(argc != 4) usage();
		connect();

		bool status = ClLoop::nms.RetrieveFile(arg1, arg2);
		CcCore::Exit(status);

	} else if(command.compare("fork") == 0) {
		connect();

		int pid;
		status = ClLoop::pro.ForkAndCheck(&pid);
		processing(status);
		if(status == ClProLang::Successful)
			cout << pid << endl;
		CcCore::Exit(status);

	} else if(command.compare("exec") == 0) {
		if(argc != 4) usage();
		connect();
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = ClLoop::pro.Exec(pid, arg2);
		processing(status);
		CcCore::Exit(status);
	
	} else if(command.compare("isalive") == 0) {
		if(argc != 3) usage();
		connect();
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = ClLoop::pro.IsAlive(pid);
		processing(status);
		if(status == ClProLang::Successful) 
			cout << "alive" << endl;
		else if(status == ClProLang::IsDead)
			cout << "dead" << endl;
		CcCore::Exit(status);

	} else if(command.compare("terminate") == 0) {
		if(argc != 3) usage();
		connect();
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = ClLoop::pro.Terminate(pid);
		processing(status);
		CcCore::Exit(status);

	} else if(command.compare("directory") == 0) {
		if(argc != 4) usage();
		connect();

		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = ClLoop::pro.Directory(pid, arg2);
		processing(status);
		CcCore::Exit(status);
	
	} else if(command.compare("include") == 0) {
		if(argc != 4) usage();
		connect();
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = ClLoop::pro.Include(pid, arg1);
		processing(status);
		CcCore::Exit(status);
	
	} else if(command.compare("include2") == 0) {
		if(argc != 5) usage();
		connect();
		
		int pid = -1;
		sscanf(arg1.c_str(), "%d", &pid);
		status = ClLoop::pro.Include(pid, arg1, arg2);
		processing(status);
		CcCore::Exit(status);

	} else {
		usage();
	}
	ClLoop::Disconnect();
	CcCore::Exit(0);
}
