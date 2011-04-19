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

void usage(void) { 
	printf("Usage: cl_ticmonitor [OPTION]...\n\n");
	printf("  -p       TCP port (9500 default)\n");
	printf("  -n       TCP server name (/feedback0 default)\n");
	printf("  -h       display this help and exit\n");
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

	CcCore::OpenLogger("cl_ticmonitor");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	ICMessage message;
	ICSerializerRapid serializer(&message);

	std::string absolute, relative;

	ClTobiIc ic;
	while(true) { 
		// Open iC
		if(ic.Attach(optport, optname) == false) {
			ic.Detach();
			if(CcCore::receivedSIGAny.Get())
				goto shutdown;
			CcTime::Sleep(5000);
			continue;
		}

		// Wait for the first message to arrive
		CcLogInfo("Waiting for endpoint...");
		while(ic.WaitMessage(&serializer) != ClTobiIc::HasMessage) {
			if(CcCore::receivedSIGAny.Get())
				goto shutdown;
			CcTime::Sleep(1000.00f);
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

			CcLogInfoS("TiC message: " << 
					" Classifiers=" << classifiers <<
					", Total=" << total <<
					", Block " << message.GetBlockIdx() << 
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
