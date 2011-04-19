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

#include "ClProAsServer.hpp" 
#include "ClNamesClient.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcServerMulti.hpp>
#include <stdlib.h>

void usage(void) { 
	printf("Usage: cl_processing [OPTION]...\n\n");
	printf("  -p       TCP port (9100 default)\n");
	printf("  -h       display this help and CcCore::Exit(\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optopt;
	CcPort optport("9100");

	while((opt = getopt(argc, argv, "p:h")) != -1) {
		if(opt == 'p')
			optport.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}
	
	CcCore::OpenLogger("cl_processing");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	
	CcLogInfoS("Processing configured: " << optport << "/TCP");
	
	// Setup TCP server
	CcServerMulti server(true, 2*CCCORE_1MB);
	ClProAsServer handler;
	ClNamesClient nsclient;

	try { 
		handler.Register(&server);
		server.Bind(CcEndpoint("0.0.0.0", optport), 2);
	} catch(CcException e) {
		CcLogFatal("Cannot bind socket");
		CcCore::Exit(1);
	}

	if(nsclient.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		CcCore::Exit(2);
	}

	int nsstatus = nsclient.Set("/processing", server.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		CcCore::Exit(3);
	}

	while(true) {
		if(nsclient.Connect() == false)  {
			CcLogFatal("Lost connection with nameserver");
			break;
		}
		if(CcCore::receivedSIGINT.Get()) 
			break;
		if(CcCore::receivedSIGTERM.Get()) 
			break;
		CcTime::Sleep(1000.00f);
	}
	nsclient.Unset("/processing");
	server.Release();

	CcCore::Exit(0);
}
