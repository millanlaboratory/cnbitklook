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
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void usage(void) { 
	printf("Usage: cl_processing [-a IP:PORT]\n");
	printf("Where: -a       127.0.0.1:9100 (default)\n");
	exit(1);
}

int main(int argc, char* argv[]) {
	// Parse command line
	int opt;
	std::string optopt;
	CcEndpoint optendpoint("127.0.0.1:9100");
	
	while((opt = getopt(argc, argv, "a:h")) != -1) {
		if(opt == 'a')
			optendpoint.SetAddress(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}
	
	CcCore::OpenLogger("cl_processing");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	
	CcLogInfo(std::string("Processing configured: ").
			append(optendpoint.GetAddress()).append("/TCP"));
	
	// Setup TCP server
	CcServerMulti server(true, 5*CCCORE_1MB);
	ClProAsServer handler;
	ClNamesClient nsclient;
	try { 
		handler.Register(&server);
		server.Bind(optendpoint, 2);
	} catch(CcException e) {
		CcLogFatal("Cannot bind socket");
		exit(1);
	}
	if(nsclient.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		exit(2);
	}
	int nsstatus = nsclient.Set("/processing", server.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		exit(3);
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
		CcTime::Sleep(2000);
	}
	nsclient.Unset("/processing");
	server.Release();

	return 0;
}
