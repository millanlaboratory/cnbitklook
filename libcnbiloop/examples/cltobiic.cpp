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

#include "ClTobiIc.hpp"
#include <libtobiic/ICMessage.hpp>
#include <libtobiic/ICSerializerRapid.hpp>
#include <iostream>

using namespace std;

void usage(void) { 
	printf("Usage: cl_ticmonitor -p TCPPORT -n NAME\n");
	printf("Where: -p       9500 (default)\n");
	printf("Where: -n       /feedback0 (default)\n");
	exit(1);
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname("/feedback0");
	CcPort optport("9500");
	
	while((opt = getopt(argc, argv, "p:n:h")) != -1) {
		if(opt == 'p')
			optport.assign(optarg);
		else if(opt == 'n')
			optname.assign(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	CcCore::OpenLogger("cltobiic");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	ICMessage message;
	ICSerializerRapid serializer(&message);

	std::stringstream stream;
	int blockidx;
	std::string absolute, relative;

	ClTobiIc ic;
	while(true) { 
		// Open iC
		if(ic.Attach(optport, optname) == false) {
			CcLogFatal("Cannot open");
			ic.Detach();
			return 1;
		}

		// Wait for the first message to arrive
		while(ic.WaitMessage(&serializer) != ClTobiIc::HasMessage) {
			CcLogInfo("Waiting for endpoint...");
			CcTime::Sleep(1000.00f);
			if(CcCore::receivedSIGAny.Get()) {
				goto shutdown;
			}
		}

		int status = ClTobiIc::NoMessage;
		while(true) { 
			status = ic.GetMessage(&serializer);
			if(status == ClTobiIc::NoMessage) {
				CcLogWarning("Endpoint attached, waiting for iC flow");
				continue;
			}
			if(status == ClTobiIc::Detached) {
				CcLogWarning("Endpoint detached");
				break;
			}
			if(CcCore::receivedSIGAny.Get())
				goto shutdown;
			if(ClLoop::IsConnected() == false) {
				CcLogWarning("Cannot connect to loop");
				goto shutdown;
			}
			
			blockidx = message.GetBlockIdx();
			message.absolute.Get(&absolute);
			message.relative.Get(&relative);
			
			std::string classifiers;
			int total = message.classifiers.Size();
			ICClassifierIter it = message.classifiers.Begin();
			while(it != message.classifiers.End()) {
				classifiers.append((*it).first);
				if(it++ != message.classifiers.Begin())
					classifiers.append(",");
			}

			CcLogInfoS(stream, "TiC message: " << 
					" Classifiers=" << classifiers <<
					", Total=" << total <<
					", Block " << blockidx << 
					", A/R=" << absolute << 
					"/" << relative);
		}
		CcLogFatal("Connection lost");
		ic.Detach();
	}

shutdown:
	ic.Detach();

	return 0;
}
