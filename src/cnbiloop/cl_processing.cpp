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

#include "ClProAsServer.hpp" 
#include "ClNamesClient.hpp"
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcServerMulti.hpp>
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
	CcServerMulti server(true, 50.00f, 5.00f, CCCORE_1MB);
	ClProAsServer handler;
	ClNamesClient nsclient;

	try { 
		handler.Register(&server);
		server.Bind(CcEndpoint("0.0.0.0", optport), 2);
	} catch(CcException e) {
		CcLogFatal("Cannot bind socket");
		CcCore::Exit(2);
	}

	if(nsclient.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		CcCore::Exit(3);
	}

	int nsstatus = nsclient.Set("/processing", server.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		CcCore::Exit(4);
	}

	while(true) {
		if(nsclient.IsConnected() == false)  {
			CcLogFatal("Lost connection with nameserver");
			break;
		}
		if(CcCore::receivedSIGINT.Get()) 
			break;
		if(CcCore::receivedSIGTERM.Get()) 
			break;
		CcTime::Sleep(500.00f);
	}
	nsclient.Unset("/processing");
	server.Release();

	CcCore::Exit(0);
}
