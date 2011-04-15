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

#include "ClTobiId.hpp"
#include <iostream>

using namespace std;

void usage(void) { 
	printf("Usage: cl_tidmonitor -n NAME\n");
	printf("Where: -n       /acquisition (default)\n");
	exit(1);
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname("/acquisition");
	
	while((opt = getopt(argc, argv, "n:h")) != -1) {
		if(opt == 'n')
			optname.assign(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	CcCore::OpenLogger("cl_tidmonitor");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	IDMessage message;
	IDSerializerRapid serializer(&message);

	std::stringstream stream;
	std::string absolute, relative;

	ClTobiId id;
	while(true) { 
		if(id.Attach(optname) == false) {
			id.Detach();
			CcTime::Sleep(5000);
			continue;
		}
	
		while(id.IsAttached() == true) { 
			if(id.Count() == 0) {
				CcTime::Sleep(500.00f);
				if(CcCore::receivedSIGAny.Get())
					goto shutdown;
				continue;
			}
			
			if(CcCore::receivedSIGAny.Get())
				goto shutdown;
		
			while(id.GetMessage(&serializer) == true) {
				message.absolute.Get(&absolute);
				message.relative.Get(&relative);

				CcLogInfoS(stream, "TiD event:" << 
						", Family=" << message.GetFamily() << 
						", Event=" << message.GetEvent() << 
						", Description=" << message.GetDescription() <<
						", Block=" << message.GetBlockIdx() <<
						", A/R=" << absolute << 
						"/" << relative);
			}
		}
		id.Detach();
	}

shutdown:
	id.Detach();

	return 0;
}
