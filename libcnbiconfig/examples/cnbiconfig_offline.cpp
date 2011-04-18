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
	 * identified by blockname and tasksetname
	 */
	CCfgTaskset* offline = NULL;
	try { 
		offline = config.OfflineEx(blockname, tasksetname);
		cout << "Taskset assembled" << endl;
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
		return 1;
	}

	/* So far so good. Each tasket has several tasks, such as
	 * right/left mi.  The number of tasks is not fixed.
	 */
	printf("Taskset: Name=%s, Description=%s, Tasks=%d\n", 
			offline->name.c_str(), 
			offline->description.c_str(), 
			offline->Count());

	/* Ok. Let's now imagine we want to configure an SMR fake feedback.
	 * We need to know:
	 * - How many tasks (see use of Count() above)?
	 * - How many trials per task?
	 * - Which tasks are we using?
	 * The tasks are in CCfgTaskset::tasks. That's a map of tasks, where the
	 * primary key is the name of the task. Let's see the content.
	 */

	CCfgTasksetConstIt it;
	for(it = offline->Begin(); it != offline->End(); it++)
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
	 */
	float threshold = 0.00f;
	try {
		threshold = 
			config.RootEx()->
			GoEx("offline")->
			GoEx(blockname)->
			QuickFloatEx("threshold");
	} catch(XMLException e) {
		cout << "Exception: " << e << endl;
		return 1;
	}
	printf("Threshold is: %f\n", threshold);	
	
	
	/* Now I need to assemble a trials vector to pass to CSmrCopilot.
	 * Watch out that here we access std::maps directly, like:
	 *  tasks[name]->config["trials"]
	 * If the key is not there, it will segfault. Please check with the
	 * iterators in real life;
	 */
	unsigned int* trials = new unsigned int(offline->Count());
	HWTrigger* triggers = new HWTrigger(offline->Count());
	GDFEvent* events = new GDFEvent(offline->Count());
	for(it = offline->Begin(); it != offline->End(); it++) {
		CCfgTask* task = it->second;
		if(task->HasConfig("trials") == false)
			cout << "Per-task configuration trials is missing" << endl;
		trials[task->id] = task->config["trials"].Int();
		triggers[task->id] = task->hwt;
		events[task->id] = task->gdf;
	}

	/* Good. We now have the trials, triggers and events, all stored in the 
	 * 3 arrays. This means we can go online! Yuppi.
	 */
	for(unsigned int i = 0; i < offline->Count(); i++) { 
		printf("ID=%d, Trials=%d, HWT=%d, GDF=%d\n", 
				i, trials[i], triggers[i], events[i]);
	}

	return 0;
}
