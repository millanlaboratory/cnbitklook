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
#include "ClLoopConfigSrv.hpp"
#include "ClNmsServer.hpp" 
#include "ClNmsClient.hpp"
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcServer.hpp>
#include <stdlib.h>
#include <getopt.h>

void usage(void) { 
	printf("Usage: cl_nameserver [OPTION]...\n\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optopt;
	
	while((opt = getopt(argc, argv, "h")) != -1) {
		if(opt == 'h') {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}
	
	CcCore::OpenLogger("cl_nameserver");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	ClLoop::Configure("", true);
	
	CcLogConfigS("Nms will bind: " << ClLoopConfigSrv::GetSrvNms());

	// Setup TCP server
	CcServer server(CCCORE_1MB);
	ClNmsServer handler;
	handler.StartMonitor();
	handler.Register(&server);
		
	if(server.Bind(ClLoopConfigSrv::GetSrvNms()) == false) {
		CcLogFatal("Cannot bind nameserver socket");
		CcCore::Exit(2);
	}

	CcTime::Sleep(1000.00f);
	ClNmsClient nsclient;
	if(nsclient.Connect(ClLoopConfigSrv::GetNms()) == false) {
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
