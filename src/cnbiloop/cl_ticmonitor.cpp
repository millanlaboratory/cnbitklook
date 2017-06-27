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
#include "ClTobiIc.hpp"
#include <getopt.h>

void usage(void) { 
	printf("Usage: cl_ticmonitor [OPTION]...\n\n");
	printf("  -a       address of nameserver in ip:port format\n");
	printf("  -p       TCP port (9500 default)\n");
	printf("  -n       TCP server name (/ctrl0 default)\n");
	printf("  -h       display this help and exit\n");
	printf("  -l       lock on iC stream\n");
	printf("  -d       dump iC message instead of logging\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname("/ctrl0");
	CcPort optport("");
	CcAddress nameserver;
	bool locking = false, dump = false;
	
	while((opt = getopt(argc, argv, "a:p:n:hld")) != -1) {
		if(opt == 'p')
			optport.assign(optarg);
		else if(opt == 'n')
			optname.assign(optarg);
		else if(opt == 'l')
			locking = true;
		else if(opt == 'd')
			dump = true;
		else if(opt == 'a')
			nameserver.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	CcCore::OpenLogger("cl_ticmonitor");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	ClLoop::Configure(nameserver);

	ICMessage message;
	ICSerializerRapid serializer(&message);

	ClTobiIc ic;
	std::string absolute, relative;
	bool attached;
	int status;

	if(ClLoop::Connect() == false) {
		CcLogFatal("Cannot connect to loop");
		goto shutdown;
	}
		
	attached = optport.empty() ? ic.Attach(optname) : ic.Attach(optport, optname);
	if(attached == false) {
		CcLogFatal("Cannot attach iC");
		goto shutdown;
	}

	CcLogInfoS("Waiting for iC messages " << (locking ? "(locking)" : "(not locking)"));
	while(true) {
		if(CcCore::receivedSIGAny.Get())
			goto shutdown;

		if(locking == false)
			status = ic.GetMessage(&serializer);
		else
			status = ic.WaitMessage(&serializer);
	
		if(ClLoop::IsConnected() == false) {
			CcLogFatal("Lost connection with loop");
			goto shutdown;
		}
		if(status == ClTobiIc::Detached) {
			CcLogFatal("iC detached");
			goto shutdown;
		} else if(status == ClTobiIc::NoMessage) {
			CcTime::Sleep(25.00f);
			continue;
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
	
		if(dump == false) {
			CcLogInfoS("Classifiers=" << classifiers <<
					", Total=" << total <<
					", Block=" << message.GetBlockIdx() << 
					", A=" << absolute << 
					", R=" << relative);
		} else {
			message.Dump();
		}
	}
shutdown:
	ic.Detach();
	CcCore::Exit(0);
}
