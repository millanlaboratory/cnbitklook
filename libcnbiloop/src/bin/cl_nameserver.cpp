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

#include "ClNamesAsServer.hpp" 
#include "ClNamesClient.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcServerMulti.hpp>
#include <stdlib.h>

void usage(void) { 
	printf("Usage: cl_nameserver [OPTION]...\n\n");
	printf("  -p       TCP port (8000 default)\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optopt;
	CcPort optport("8000");
	
	while((opt = getopt(argc, argv, "p:h")) != -1) {
		if(opt == 'p')
			optport.assign(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}
	
	CcCore::OpenLogger("cl_nameserver");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	CcLogInfoS("Nameserver configured: " << optport << "/TCP");
	
	// Setup TCP server
	CcEndpoint endpoint("0.0.0.0", optport);
	CcServerMulti server(true, 5*CCCORE_1MB);
	ClNamesAsServer handler;
	ClNamesClient nsclient;
	handler.StartMonitor();
	try {
		handler.Register(&server);
		server.Bind(endpoint, 2);
	} catch(CcException e) {
		CcLogFatal("Cannot bind socket");
		exit(1);
	}

	CcTime::Sleep(1000.00f);
	if(nsclient.Connect(endpoint.GetAddress()) == false) {
		CcLogFatal("Cannot connect to nameserver");
		exit(2);
	}
	
	int nsstatus = nsclient.Set("/nameserver", server.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		exit(3);
	}
	
	while(true) {
		if(CcCore::receivedSIGINT.Get()) 
			break;
		if(CcCore::receivedSIGTERM.Get()) 
			break;
		CcTime::Sleep(1000.00f);
	}
	nsclient.Unset("/nameserver");
	server.Release();

	return 0;
}
