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

void usage(void) { 
	printf("Usage: cl_tidmonitor [OPTION]...\n\n");
	printf("  -n       bus name (/bus default)\n");
	printf("  -h       display this help and exit\n");
	CcCore::Exit(1);
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname("/bus");
	
	while((opt = getopt(argc, argv, "n:h")) != -1) {
		if(opt == 'n')
			optname.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	CcCore::OpenLogger("cl_tidmonitor");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	IDMessage messageI;
	IDSerializerRapid serializerI(&messageI);

#ifdef ENABLE_BOUNCE
	IDMessage messageO;
	messageO.SetDescription("cl_tidmonitor");
	messageO.SetFamilyType(IDMessage::FamilyBiosig);
	messageO.SetEvent(0xFFFF);
	IDSerializerRapid serializerO(&messageO);
#endif

	ClTobiId id(ClTobiId::SetGet);
	std::string absolute, relative;
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
				CcTime::Sleep(250.00f);
				if(CcCore::receivedSIGAny.Get())
					goto shutdown;
				continue;
			}

#ifdef ENABLE_BOUNCE
			if(id.SetMessage(&serializerO) == false) {
				CcLogFatalS("Cannot set iD event: " << messageO.GetEvent());
				goto shutdown;
			}
#endif
			
			if(CcCore::receivedSIGAny.Get())
				goto shutdown;
			
			while(id.GetMessage(&serializerI) == true) {
				messageI.absolute.Get(&absolute);
				messageI.relative.Get(&relative);

				CcLogInfoS("TiD event:" << 
						" Family=" << messageI.GetFamily() << 
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
	CcCore::Exit(0);
}
