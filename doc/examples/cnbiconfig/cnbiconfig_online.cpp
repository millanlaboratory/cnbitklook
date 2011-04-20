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

#include <libcnbiconfig/CCfgConfig.hpp>
#include <libtobiic/ICMessage.hpp>
#include <libtobiic/ICSerializerRapid.hpp>
#include <iostream>
#include <string>

using namespace std;

int main(void) {
	std::string blockname("mi"), tasksetname("mi_rhlh");
	
	CCfgConfig config;
	try {
		config.ImportFileEx("../extra/example.xml");
		cout << "File loaded" << endl;
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
		return 1;
	}

	if(config.Validate() == false) {
		cout << "XML file is not valid\n";
		return 1;
	}
			

	/* We will now ask CCfgConfig to return the tasket 
	 * identified by blockname and tasksetname.
	 * We also pass an ICMessage so that it gets configured depending on the
	 * taskset.
	 */
	CCfgTaskset* online = NULL;
	ICMessage message;
	try { 
		online = config.OnlineEx(blockname, tasksetname, &message);
		cout << "Taskset assembled" << endl;
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
		return 1;
	}
	message.Dump();

	/* So far so good. Each tasket has several tasks, such as
	 * right/left mi.  The number of tasks is not fixed.
	 */
	printf("Taskset: Name=%s, Description=%s, Tasks=%d\n", 
			online->name.c_str(), 
			online->description.c_str(), 
			online->Count());

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

	/* I imagine I need to configure the CSmrCopilot. First thing, I need to get
	 * the unique threshold we use for training.
	 *
	 * I heavily use CCfgXMLConfig here. The Ex methods raise exceptions. 
	 *
	 * Here we try to get a unique-per block threshold. It will fail.
	 */
	float threshold = 0.00f;
	try {
		threshold = 
			config.RootEx()->
			GoEx("online")->
			GoEx(blockname)->
			QuickFloatEx("threshold");
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
	}
	printf("Threshold is: %f\n", threshold);	
	
	
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
