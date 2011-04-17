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

	IDMessage messageI, messageO;
	IDSerializerRapid serializerI(&messageI), serializerO(&messageO);

	messageO.SetDescription("cl_tidmonitor");
	messageO.SetFamilyType(IDMessage::FamilyBiosig);
	messageO.SetEvent(0xFFFF);

	std::string absolute, relative;

	ClTobiId id;
	while(true) { 
		if(id.Attach(optname) == false) {
			if(CcCore::receivedSIGAny.Get())
				goto shutdown;
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
			
			if(id.SendMessage(&serializerO) == false) {
				CcLogFatalS("Cannot send iD event: " << messageO.GetEvent());
				goto shutdown;
			}
		
			while(id.GetMessage(&serializerI) == true) {
				messageI.absolute.Get(&absolute);
				messageI.relative.Get(&relative);

				CcLogInfoS("TiD event:" << 
						", Family=" << messageI.GetFamily() << 
						", Event=" << messageI.GetEvent() << 
						", Description=" << messageI.GetDescription() <<
						", Block=" << messageI.GetBlockIdx() <<
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
