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

#include "ClNmsServer.hpp" 
#include "ClNmsClient.hpp"
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcServer.hpp>
#include <stdlib.h>

void usage(void) { 
	printf("Usage: cl_nameserver [OPTION]...\n\n");
	printf("  -p       nameserver TCP port (8123 default)\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optopt;
	CcPort portNameserver("8123");
	
	while((opt = getopt(argc, argv, "p:h")) != -1) {
		if(opt == 'p')
			portNameserver.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}
	
	CcCore::OpenLogger("cl_nameserver");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	
	// Handle the CNBITK_ADDRESS envvar
	CcIp cnbitkip = CcCore::GetEnvCnbiTkAddress();
	if(cnbitkip.empty() == true) {
		cnbitkip.assign("127.0.0.1");
	}
	CcLogConfigS("CnbkTk loop running on: " << cnbitkip);

	// Handle hosts
	CcEndpoint epNameserver(cnbitkip, portNameserver);
	CcLogConfigS("Nameserver will bind: " << epNameserver.GetAddress());

	CcServer server(CCCORE_1MB);
	ClNmsServer handler;
	ClNmsClient nsclient;
	handler.StartMonitor();
	try {
		handler.Register(&server);
		server.Bind(epNameserver.GetAddress());
	} catch(CcException e) {
		CcLogFatal("Cannot bind socket");
		CcCore::Exit(2);
	}

	CcTime::Sleep(1000.00f);
	if(nsclient.Connect(epNameserver.GetAddress()) == false) {
		CcLogFatal("Cannot connect to nameserver");
		CcCore::Exit(3);
	}
	
	int nsstatus = nsclient.Set("/nameserver", server.GetLocal());
	if(nsstatus != ClNmsLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		CcCore::Exit(4);
	}
	
	while(true) {
		if(CcCore::receivedSIGINT.Get()) 
			break;
		if(CcCore::receivedSIGTERM.Get()) 
			break;
		CcTime::Sleep(500.00f);
	}
	nsclient.Unset("/nameserver");
	server.Release();
	
	CcCore::Exit(0);
}
