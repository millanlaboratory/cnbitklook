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
		CCfgConfig config;
		config.ImportFileEx("../extra/example.xml");

		if(config.Validate() == false) {
			cout << "XML file is not valid\n";
			return -1;
		}
		cout << "XML file is valid\n";
		
		CCfgTaskset* offlineMI = NULL;
		offlineMI = config.OfflineEx("smr", "mi_rhlh");
		
		ICMessage sender;
		CCfgTaskset* onlineMI = config.OnlineEx("smr", "mi_rhlh", &sender);
		CCfgTaskset* onlineErrP = config.OnlineEx("smrerrp", "errp", &sender);


		cout << ">>>> MI classifier: " << onlineMI->classifier << endl;
		cout << ">>>> ErrP classifier: " << onlineErrP->classifier << endl;

		onlineMI->GetEx("mi_hand_right")->Dump();
		onlineMI->GetEx("mi_hand_left")->Dump();
		cout << "Integration: " << config.RootEx()->GoEx("online")->
			GoEx("smr")->GoEx("integration")->GetEx().Float() << endl;
		//onlineMI->GetEx("mi_hand_left")->Dump();
		onlineErrP->GetEx("errp_true")->Dump();
		cout << "Threshold: " << 
			onlineMI->GetEx("mi_hand_left")->config["threshold"].Float() << endl;
		
		cout << "By id: " << onlineMI->Count() << endl;
		CCfgTask* t = NULL;
		t = onlineMI->GetEx(1);
		t->Dump();
		t = onlineMI->GetEx(0);
		t->Dump();

		cout << "Go: device is: " << config.RootEx()->GoEx("options")->
			GoEx("fes")->GoEx("dev")->GetEx().String() << endl;
		cout << "Quick: device is: " << 
			config.RootEx()->QuickEx("options/fes/dev")->GetEx().String() << endl;
		cout << "QuickString: device is: " << 
			config.RootEx()->QuickStringEx("options/fes/dev") << endl;
		cout << "GDF off: " << 
			config.RootEx()->QuickUIntEx("protocol/gdfevents/off") << endl;
		cout << "GDF cfeedback: " << 
			config.RootEx()->QuickUIntEx("protocol/gdfevents/cfeedback") << endl;
		cout << "GDF cfeedbackoff: " << 
			config.RootEx()->QuickUIntEx("protocol/gdfevents/cfeedbackoff") << endl;

		
		std::string buffer;
		ICSerializerRapid serializer(&sender, true);
		serializer.Serialize(&buffer);
		
		ICMessage receiver;
		ICSerializerRapid deserializer(&receiver, true);
		deserializer.Deserialize(&buffer);
		
		sender.Dump();	
		receiver.Dump();

		cout << "Subject is: " << 
			config.RootEx()->GoEx("subject")->GoEx("id")->GetEx().String() << endl;
		cout << "Error is: " << endl; 
		cout << config.RootEx()->GoEx("subject")->GoEx("name")->GetEx().String() << endl;
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
	}
	return 0;
}
