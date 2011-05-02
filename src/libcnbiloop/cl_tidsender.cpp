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
#include <stdio.h>

void usage(void) { 
	printf("Usage: cl_tidsender [OPTION]...\n\n");
	printf("  -n       acquisition name (/acquisition default)\n");
	printf("  -e       GDF event (default 666)\n");
	printf("  -h       display this help and exit\n");
	CcCore::Exit(1);
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname("/acquisition");
	//GDFEvent event = 666;
	unsigned int event = 666;
	
	while((opt = getopt(argc, argv, "n:e:h")) != -1) {
		if(opt == 'n') {
			optname.assign(optarg);
		} else if(opt == 'e') {
			sscanf(optarg, "%u", &event); 
		} else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	CcCore::OpenLogger("cl_tidsender");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	IDMessage messageI;
	IDSerializerRapid serializerI(&messageI);
	messageI.SetFamilyType(IDMessage::FamilyBiosig);
	messageI.SetEvent(event);

	ClTobiId id(ClTobiId::SetOnly);
	std::string absolute, relative;
	while(true) { 
		if(id.Attach(optname) == false) {
			if(CcCore::receivedSIGAny.Get())
				goto shutdown;
			id.Detach();
			CcTime::Sleep(5000);
			continue;
		}
		
		CcTime::Sleep(2000.00f);
		printf("\n");
		while(id.IsAttached() == true) { 
			printf("Press 'Enter' to send GDF=%u ('q' to quit): ", event);
			switch(getchar()) {
				case 'q':
					goto shutdown;
					break;
				default:
					break;
			}

			id.SetMessage(&serializerI);

			if(CcCore::receivedSIGAny.Get()) {
				CcLogWarning("User asked to go down");
				goto shutdown;
			}
		}
		id.Detach();
	}


shutdown:
	id.Detach();
	CcCore::Exit(0);
}
