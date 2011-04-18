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
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
		
bool CCfgConfig::Validate(void) {
	try {
		// Test main nodes
		CCfgXMLConfig::RootEx()->GoEx("recording");
		CCfgXMLConfig::RootEx()->GoEx("classifier");
		CCfgXMLConfig::RootEx()->GoEx("configuration");
		CCfgXMLConfig::RootEx()->GoEx("options");
		CCfgXMLConfig::RootEx()->GoEx("offline");
		CCfgXMLConfig::RootEx()->GoEx("online");
		CCfgXMLConfig::RootEx()->GoEx("parameters");
		CCfgXMLConfig::RootEx()->GoEx("subject");
		CCfgXMLConfig::RootEx()->GoEx("tasks");
		CCfgXMLConfig::RootEx()->GoEx("events");
		CCfgXMLConfig::RootEx()->GoEx("taskset");
		CCfgXMLConfig::RootEx()->GoEx("protocol");
		
		// Test mandatory nodes
		CCfgXMLConfig::RootEx()->QuickEx("subject/id");
		CCfgXMLConfig::RootEx()->QuickEx("subject/age");
		CCfgXMLConfig::RootEx()->QuickEx("recording/experimenter");
		CCfgXMLConfig::RootEx()->QuickEx("recording/location");
		CCfgXMLConfig::RootEx()->QuickEx("recording/project");
		CCfgXMLConfig::RootEx()->QuickEx("recording/experiment");
		CCfgXMLConfig::RootEx()->QuickEx("recording/date");
	} catch(XMLException e) {
		std::cout << e << std::endl;
		return false;
	}
	return true;
}
		
CCfgTaskset* CCfgConfig::OnlineEx(const std::string& blockname, 
		const std::string& taskset, ICMessage* icmessage) {
	CCfgTaskset* tasks = new CCfgTaskset(taskset);
	try {
		CCfgXMLConfig::RootEx()->GoEx("online")->GoEx(blockname)->SetBranch();
		tasks->description = 
			CCfgXMLConfig::BranchEx()->GetAttrEx("description");
		this->ParseTasksetEx(taskset, tasks);
		this->ParseClassifierEx(blockname, taskset, tasks, icmessage);
		this->ParseConfigEx("online", blockname, taskset, tasks);
	} catch(XMLException e) {
		delete tasks;
		throw e;
	}

	return tasks;
}

CCfgTaskset* CCfgConfig::OfflineEx(const std::string& offline,
		const std::string& taskset) {
	CCfgTaskset* tasks = new CCfgTaskset(taskset);
	try {
		CCfgXMLConfig::RootEx()->GoEx("offline")->GoEx(offline)->SetBranch();
		tasks->description = 
			CCfgXMLConfig::BranchEx()->GetAttrEx("description");
		this->ParseTasksetEx(taskset, tasks);
		this->ParseConfigEx("offline", offline, taskset, tasks);
	} catch(XMLException e) {
		delete tasks;
		throw e;
	}
	
	return tasks;
}


void CCfgConfig::ParseTasksetEx(const std::string& name, CCfgTaskset* taskset) {
	// Use LibCCfgXMLConfig::XMLType for parsing
	XMLType converter;
	
	// Find the taskset branch and start looping
	CCfgXMLConfig::RootEx()->GoEx("taskset")->GoEx(name)->SetBranch();
	XMLNode node = CCfgXMLConfig::Child();
	while(true) {
		if(node == NULL)
			break;

		std::string classKey, classId, classTrials;
		// TODO: try/catch
		classTrials.assign(node->value());
		classKey.assign(node->first_attribute("key")->value());
		classId.assign(node->first_attribute("id")->value());

		// Build a CCfgTask object out of the taskset
		CCfgTask* task = new CCfgTask;
		task->name = classKey;
		converter.Guess(classId);
		task->id = (unsigned int)converter.Int();
		converter.Guess(classTrials);
		task->trials = (unsigned int)converter.Int();

		// Fill up missing informations
		XMLNode nTasks = CCfgXMLConfig::_document.doc.
			first_node(CCfgXMLConfig::_rootname.c_str())->first_node("tasks");
		nTasks = nTasks->first_node(classKey.c_str());
		if(nTasks == NULL) {
			std::string info;
			info += "Task '";
			info += classKey;
			info += "' not found for taskset '";
			info += name;
			info += "'";
			throw XMLException(info, __PRETTY_FUNCTION__);
		}
		
		task->description.assign(nTasks->first_node("description")->value());
		converter.Guess(nTasks->first_node("hwtrigger")->value());
		task->hwt = converter.Int();
		converter.Guess(nTasks->first_node("gdfevent")->value());
		task->gdf = converter.UInt();
		
		// Finally add task to taskset
		taskset->AddTask(task);

		// Loop for next
		node = CCfgXMLConfig::NextSibling();
	}
}


void CCfgConfig::ParseConfigEx(const std::string& mode, const std::string& modename,
		const std::string& tasksetname, CCfgTaskset* taskset) {
	CCfgXMLConfig::RootEx()->GoEx(mode)->GoEx(modename)->GoEx("taskset")->SetBranch();
	XMLNode node = CCfgXMLConfig::_nBranch;
	
	std::string tname;
	while(node != NULL) {
		tname = CCfgXMLConfig::GetAttrEx("key");
		if(tname.compare(tasksetname) == 0) {
			node = CCfgXMLConfig::Child();
			break;
		}
		node = CCfgXMLConfig::NextSibling();
	}
	if(node == NULL) {
		std::string info;
		info += "Taskset ";
		info += tasksetname;
		info += " not found in mode ";
		info += modename;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	while(true) {
		if(node == NULL)
			break;

		std::string name(node->name());
		node = node->first_node();
		while(true) {
			if(node == NULL)
				break;
			
			std::string taskName(node->name());
			CCfgTask* task = taskset->GetTaskEx(taskName);
			task->config[name] = XMLType(node->value()); 
			
			// Loop for next
			node = node->next_sibling();
		}
		node = CCfgXMLConfig::NextSibling();
	}
}
		
void CCfgConfig::ParseClassifierEx(const std::string& modename, 
		const std::string& tasksetname, CCfgTaskset* taskset, 
		ICMessage* icmessage) {
	CCfgXMLConfig::RootEx()->GoEx("online")->GoEx(modename)->GoEx("taskset")->SetBranch();
	XMLNode node = CCfgXMLConfig::_nBranch;
	
	std::string tname;
	std::string classifiername;
	while(node != NULL) {
		tname = CCfgXMLConfig::GetAttrEx("key");
		if(tname.compare(tasksetname) == 0) {
			classifiername = CCfgXMLConfig::GetAttrEx("classifier");
			taskset->classifier.assign(classifiername);
			node = CCfgXMLConfig::Child();
			break;
		}
		node = CCfgXMLConfig::NextSibling();
	}
	if(node == NULL) {
		std::string info;
		info += "Taskset ";
		info += tasksetname;
		info += " not found in mode ";
		info += modename;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	if(icmessage == NULL)
		return;

	CCfgXMLConfig::RootEx()->GoEx("classifier")->GoEx(classifiername)->SetBranch();
	std::string description = CCfgXMLConfig::BranchEx()->QuickStringEx("description");
	std::string valuetype = CCfgXMLConfig::BranchEx()->QuickStringEx("icvalue");
	std::string labeltype = CCfgXMLConfig::BranchEx()->QuickStringEx("iclabel");
	ICClassifier* classifier = new ICClassifier(classifiername, description);
	if(classifier->SetValueType(valuetype) == false) {
		std::string info;
		info += "ICVtype not known: ";
		info += valuetype;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	if(classifier->SetLabelType(labeltype) == false) {
		std::string info;
		info += "ICLtype not known: ";
		info += labeltype;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	CCfgTasksetIt it = taskset->tasks.begin();
	while(it != taskset->tasks.end()) {
		std::string label;
		
		if(labeltype.compare(ICClassifier::TxtLabelBiosig) == 0) {
			char cache[16];
			sprintf(cache, "0x%X", it->second->gdf);
			label.assign(cache);
		} else {
			label.assign(it->first);
		}
		ICClass* cclass = new ICClass(label, 0.00f);
		classifier->classes.Add(cclass);
		it++;
	}

	icmessage->classifiers.Add(classifier);
}
