/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>
    Inaki Iturrate <inaki.iturrate@epfl.ch>

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

#include "ClTobiId.hpp"
#include <iostream>

void usage(void) { 
	printf("Usage: cl_tidsender [OPTION]...\n\n");
	printf("  -a       address of nameserver in ip:port format\n");
	printf("  -n       iD bus name (/bus default)\n");
	printf("  -u       Interactive events\n");	
	printf("  -e       GDF event (default 666)\n");
	printf("  -d       delay (in ms) for automatic mode (default disabled)\n");
	printf("  -h       display this help and exit\n");
	CcCore::Exit(1);
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname("/bus");
	unsigned int event = 666;
	bool interactive = false;
	CcAddress nameserver;
	
	float ms = 0.00f;
	
	while((opt = getopt(argc, argv, "n:d:e:h:u")) != -1) {
		if(opt == 'n') {
			optname.assign(optarg);
		} else if(opt == 'e') {
			sscanf(optarg, "%u", &event); 
		} else if(opt == 'd') {
			sscanf(optarg, "%f", &ms); 
		} else if(opt == 'u') {
			interactive = true; 			
		} else if(opt == 'a') {
			nameserver.assign(optarg);
		} else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	CcCore::OpenLogger("cl_tidsender");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	ClLoop::Configure(nameserver);

	IDMessage messageI;
	IDSerializerRapid serializerI(&messageI);
	messageI.SetDescription("cl_tidsender");
	messageI.SetFamilyType(IDMessage::FamilyBiosig);
	messageI.SetEvent(event);

	int fidx = TCBlock::BlockIdxUnset;

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
	
		CcLogConfigS("iD sender now attached to: " << optname);
		CcTime::Sleep(1000.00f);

		if(ms == 0) {
			if(interactive == true) {
				printf("\n\nWrite the GDF integer event to send, 'q' to quit:\n");
			} else {
				printf("\n\nPress 'enter' to send GDF=%u, 'q' to quit:\n", event);
			}
			while(id.IsAttached() == true) { 
				printf("NDF=%d >> ", fidx);
				if(interactive == true) {
					if(!(std::cin >> event)) 
						goto shutdown;
					 else 
						messageI.SetEvent(event);						
				} else {
					switch(getchar()) {
						case 'q':
							goto shutdown;
							break;
						default:
							break;
					}
				}
				id.SetMessage(&serializerI, TCBlock::BlockIdxUnset, &fidx);
			}
		} else { 
			printf("\n\nSending GDF=%u every %f ms\n", event, ms);
			while(id.IsAttached() == true) { 
				id.SetMessage(&serializerI);
				CcTime::Sleep(ms);
				if(CcCore::receivedSIGAny.Get())
					goto shutdown;
			}
		}
		id.Detach();
	}

shutdown:
	id.Detach();
	CcCore::Exit(0);
}
