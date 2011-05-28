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
	
	ClTobiId id(ClTobiId::SetGet);
	std::string absolute, relative;

	/* Dear user, Tobi iD is a async device. 
	 * So, you can poll it, checking if a message was received, or you 
	 * can register the callback on a CcProxy handler, like this:
	CcProxy handler;
	CcRegister(id.iOnHasMessage, CcProxy, &handler, Handle);
	 */

#ifdef ENABLE_BOUNCE
	IDMessage messageO;
	messageO.SetDescription("cl_tidmonitor");
	messageO.SetFamilyType(IDMessage::FamilyBiosig);
	messageO.SetEvent(0xFFFF);
	IDSerializerRapid serializerO(&messageO);
#endif
			
	if(ClLoop::Connect() == false) {
		CcLogFatal("Cannot connect to loop");
		goto shutdown;
	}

	if(id.Attach(optname) == false) {
		CcLogWarning("Cannot attach iD");
		goto shutdown;
	}

	CcLogInfo("Waiting for iD messages");
	while(true) { 
		if(ClLoop::IsConnected() == false) {
			CcLogFatal("Lost connection with loop");
			goto shutdown;
		}
		if(id.IsAttached() == false) {
			CcLogFatal("iD detached");
			goto shutdown;
		}

		if(CcCore::receivedSIGAny.Get())
			goto shutdown;
		if(id.Count() == 0) {
			CcTime::Sleep(25.00f);
			continue;
		}

#ifdef ENABLE_BOUNCE
		if(id.SetMessage(&serializerO) == false) {
			CcLogFatalS("Cannot set iD event: " << messageO.GetEvent());
			goto shutdown;
		}
#endif
		while(id.GetMessage(&serializerI) == true) {
			messageI.absolute.Get(&absolute);
			messageI.relative.Get(&relative);

			CcLogInfoS("Block=" << messageI.GetBlockIdx() <<
					", Description=" << messageI.GetDescription() <<
					", Family=" << messageI.GetFamily() << 
					", Event=" << messageI.GetEvent());
			//		", A/R=" << absolute << 
			//		"/" << relative);
		}
	}

shutdown:
	id.Detach();
	CcCore::Exit(0);
}
