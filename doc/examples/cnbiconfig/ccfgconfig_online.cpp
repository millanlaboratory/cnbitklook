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

#include <cnbiconfig/CCfgConfig.hpp>
#include <tobiic/ICMessage.hpp>
#include <tobiid/IDMessage.hpp>
#include <tobiic/ICSerializerRapid.hpp>
#include <iostream>
#include <string>

using namespace std;

#define BLOCK	"mi"
#define TAKSET	"mi_rhlh"

int main(void) {
	std::string blockname(BLOCK), tasksetname(TAKSET);
	
	CCfgConfig config;
	try {
		config.ImportFileEx("../../../../extra/cnbiconfig_example.xml");
		cout << "File loaded" << endl;
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
		return 1;
	}

	if(config.Validate() == false) {
		cout << "XML file is not valid\n";
		return 1;
	} else {
		cout << "XML is valid\n";
	}
			

	/* We will now ask CCfgConfig to return the tasket 
	 * identified by blockname and tasksetname.
	 * We also pass an ICMessage so that it gets configured depending on the
	 * taskset.
	 */
	CCfgTaskset* online = NULL;
	ICMessage icM;
	IDMessage idM;

	try { 
		cout << "Assembling taskset" << endl;
		online = config.OnlineEx(blockname, tasksetname, &icM, &idM);
		cout << "Taskset assembled" << endl;
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
		return 1;
	}
	icM.Dump();
	idM.Dump();

	/* So far so good. Each tasket has several tasks, such as
	 * right/left mi.  The number of tasks is not fixed.
	 */
	printf("Taskset: Name=%s, Description=%s, Tasks=%d\n", 
			online->name.c_str(), 
			online->description.c_str(), 
			online->Count());
	CCfgTask* temp = NULL;
	
	printf("\n\n");
	temp = online->GetTaskEx(0);
	temp->Dump();
	
	temp = online->GetTaskEx(1);
	temp->Dump();
	printf("\n\n");
	
	online->Dump();
	online->DumpClassifier();
	online->DumpNDF();

	/* Ok. Let's now imagine we want to configure an SMR fake feedback.
	 * We need to know:
	 * - How many tasks (see use of Count() above)?
	 * - How many trials per task?
	 * - Which tasks are we using?
	 * The tasks are in CCfgTaskset::tasks. That's a map of tasks, where the
	 * primary key is the name of the task. Let's see the content.
	 */

	CCfgTasksetConstIt it;
	for(it = online->Begin(); it != online->End(); it++)
		(*it).second->Dump();

	/* Cool. The complexity here is due to the nested nature of the XML
	 * configuration files, that carry a lot of presets to ease off our living.
	 * One very important property of each task (CCfgTask) is the ID.
	 * The ID can be used to "sort" the tasks. 
	 * For example, when we talk about MI right/left hand, we normally bind:
	 * - right hand as the first task
	 * - left hand as the second task
	 * IDs can then be mapped to the 4-bars feedback. 
	 * This is not necessary, you can always check the CCfgTask::name property
	 * and do what you need according to that. But with the IDs I have a simpler
	 * life in coding, so the logical mapping can somehow help you.
	 */

	
	/* Now I need to assemble a trials vector to pass to CSmrCopilot.
	 * Watch out that here we access std::maps directly, like:
	 *  tasks[name]->config["trials"]
	 * If the key is not there, it will segfault. Please check with the
	 * iterators in real life;
	 */
	try {
	float* thresholds = new float(online->Count());
	unsigned int* trials = new unsigned int(online->Count());
	HWTrigger* triggers = new HWTrigger(online->Count());
	GDFEvent* events = new GDFEvent(online->Count());
	for(it = online->Begin(); it != online->End(); it++) {
		CCfgTask* task = it->second;
		if(task->HasConfig("trials") == false)
			cout << "Per-task configuration trials is missing" << endl;
		if(task->HasConfig("threshold") == false)
			cout << "Per-task configuration threshold is missing" << endl;
		trials[task->id] = task->config["trials"].Int();
		thresholds[task->id] = task->config["threshold"].Float();
		triggers[task->id] = task->hwt;
		events[task->id] = task->gdf;
	}

	/* Good. We now have the trials, triggers and events, all stored in the 
	 * 3 arrays. This means we can go online! Yuppi.
	 */
	for(unsigned int i = 0; i < online->Count(); i++) { 
		printf("ID=%d, Trials=%d, HWT=%d, GDF=%d, Th=%f\n", 
				i, trials[i], triggers[i], events[i], thresholds[i]);
	}
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
	}



		/*
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
			config.RootEx()->QuickGDFIntEx("protocol/gdfevents/off") << endl;
		cout << "GDF cfeedback: " << 
			config.RootEx()->QuickGDFIntEx("protocol/gdfevents/cfeedback") << endl;
		cout << "GDF cfeedbackoff: " << 
			config.RootEx()->QuickGDFIntEx("protocol/gdfevents/cfeedbackoff") << endl;

		
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
		*/
	return 0;
}
