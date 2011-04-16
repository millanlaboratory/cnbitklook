/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiconfig library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CCfgConfig.hpp"
#include <libtobiic/ICMessage.hpp>
#include <libtobiic/ICSerializerRapid.hpp>
#include <iostream>
#include <string>

using namespace std;

int main(void) {
	try {
		CCfgConfig config("../extra/example.xml");
		if(config.Validate() == false) {
			cout << "XML file is not valid\n";
			return -1;
		}
		cout << "XML file is valid\n";
		
		CCfgTaskset* offlineMI = NULL;
		offlineMI = config.Offline("smr", "mi_rhlh");
		
		ICMessage sender;
		CCfgTaskset* onlineMI = config.Online("smr", "mi_rhlh", &sender);
		CCfgTaskset* onlineErrP = config.Online("smrerrp", "errp", &sender);


		cout << ">>>> MI classifier: " << onlineMI->classifier << endl;
		cout << ">>>> ErrP classifier: " << onlineErrP->classifier << endl;

		onlineMI->Get("mi_hand_right")->Dump();
		onlineMI->Get("mi_hand_left")->Dump();
		cout << "Integration: " << config.Root()->Go("online")->
			Go("smr")->Go("integration")->Get().Float() << endl;
		//onlineMI->Get("mi_hand_left")->Dump();
		onlineErrP->Get("errp_true")->Dump();
		cout << "Threshold: " << 
			onlineMI->Get("mi_hand_left")->config["threshold"].Float() << endl;
		
		cout << "By id: " << onlineMI->Count() << endl;
		CCfgTask* t = NULL;
		t = onlineMI->Get(1);
		t->Dump();
		t = onlineMI->Get(0);
		t->Dump();

		cout << "Go: device is: " << config.Root()->Go("options")->
			Go("fes")->Go("dev")->Get().String() << endl;
		cout << "Quick: device is: " << 
			config.Root()->Quick("options/fes/dev")->Get().String() << endl;
		cout << "QuickString: device is: " << 
			config.Root()->QuickString("options/fes/dev") << endl;
		cout << "GDF off: " << 
			config.Root()->QuickUInt("protocol/gdfevents/off") << endl;
		cout << "GDF cfeedback: " << 
			config.Root()->QuickUInt("protocol/gdfevents/cfeedback") << endl;
		cout << "GDF cfeedbackoff: " << 
			config.Root()->QuickUInt("protocol/gdfevents/cfeedbackoff") << endl;

		
		std::string buffer;
		ICSerializerRapid serializer(&sender, true);
		serializer.Serialize(&buffer);
		
		ICMessage receiver;
		ICSerializerRapid deserializer(&receiver, true);
		deserializer.Deserialize(&buffer);
		
		sender.Dump();	
		receiver.Dump();

		cout << "Subject is: " << 
			config.Root()->Go("subject")->Go("id")->Get().String() << endl;
		cout << "Error is: " << endl; 
		cout << config.Root()->Go("subject")->Go("name")->Get().String() << endl;
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
	}
	return 0;
}
