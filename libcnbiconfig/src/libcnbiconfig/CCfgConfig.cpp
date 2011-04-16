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

		
bool CCfgConfig::LoadFile(const std::string& filename) {
	return CCfgXMLConfig::ImportFile(filename);
}

bool CCfgConfig::LoadBuffer(const std::string& filename) {
	return CCfgXMLConfig::ImportBuffer(filename);
}
		
bool CCfgConfig::Validate(void) {
	try {
		// Test main nodes
		CCfgXMLConfig::Root()->Go("recording");
		CCfgXMLConfig::Root()->Go("classifier");
		CCfgXMLConfig::Root()->Go("configuration");
		CCfgXMLConfig::Root()->Go("options");
		CCfgXMLConfig::Root()->Go("offline");
		CCfgXMLConfig::Root()->Go("online");
		CCfgXMLConfig::Root()->Go("parameters");
		CCfgXMLConfig::Root()->Go("subject");
		CCfgXMLConfig::Root()->Go("tasks");
		CCfgXMLConfig::Root()->Go("taskset");
		CCfgXMLConfig::Root()->Go("protocol");
		
		// Test mandatory nodes
		CCfgXMLConfig::Root()->Quick("subject/id");
		CCfgXMLConfig::Root()->Quick("subject/age");
		CCfgXMLConfig::Root()->Quick("recording/experimenter");
		CCfgXMLConfig::Root()->Quick("recording/location");
		CCfgXMLConfig::Root()->Quick("recording/project");
		CCfgXMLConfig::Root()->Quick("recording/experiment");
		CCfgXMLConfig::Root()->Quick("recording/date");
	} catch(XMLException e) {
		std::cout << e << std::endl;
		return false;
	}
	return true;
}
		
CCfgTaskset* CCfgConfig::Online(const std::string& blockname, 
		const std::string& taskset, ICMessage* icmessage) {
	CCfgTaskset* tasks = new CCfgTaskset();
	CCfgXMLConfig::Root()->Go("online")->Go(blockname)->SetBranch();
	tasks->description = 
		CCfgXMLConfig::Branch()->Go("description")->Get().String();

	this->ParseTaskset(taskset, tasks);
	this->ParseClassifier(blockname, taskset, tasks, icmessage);
	this->ParseConfig("online", blockname, taskset, tasks);

	return tasks;
}

CCfgTaskset* CCfgConfig::Offline(const std::string& offline,
		const std::string& taskset) {
	CCfgTaskset* tasks = new CCfgTaskset();
	CCfgXMLConfig::Root()->Go("offline")->Go(offline)->SetBranch();
	
	tasks->description = 
		CCfgXMLConfig::Branch()->Go("description")->Get().String();
	this->ParseTaskset(taskset, tasks);
	this->ParseConfig("offline", offline, taskset, tasks);
	
	return tasks;
}


void CCfgConfig::ParseTaskset(const std::string& name, CCfgTaskset* taskset) {
	// Use LibCCfgXMLConfig::XMLType for parsing
	XMLType converter;
	
	// Find the taskset branch and start looping
	CCfgXMLConfig::Root()->Go("taskset")->Go(name)->SetBranch();
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
		taskset->Add(task);

		// Loop for next
		node = CCfgXMLConfig::NextSibling();
	}
}


void CCfgConfig::ParseConfig(const std::string& mode, const std::string& modename,
		const std::string& tasksetname, CCfgTaskset* taskset) {
	CCfgXMLConfig::Root()->Go(mode)->Go(modename)->Go("taskset")->SetBranch();
	XMLNode node = CCfgXMLConfig::_nBranch;
	
	std::string tname;
	while(node != NULL) {
		tname = CCfgXMLConfig::GetAttr("key");
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
			CCfgTask* task = taskset->Get(taskName);
			task->config[name] = XMLType(node->value()); 
			
			// Loop for next
			node = node->next_sibling();
		}
		node = CCfgXMLConfig::NextSibling();
	}
}
		
void CCfgConfig::ParseClassifier(const std::string& modename, 
		const std::string& tasksetname, CCfgTaskset* taskset, 
		ICMessage* icmessage) {
	CCfgXMLConfig::Root()->Go("online")->Go(modename)->Go("taskset")->SetBranch();
	XMLNode node = CCfgXMLConfig::_nBranch;
	
	std::string tname;
	std::string classifiername;
	while(node != NULL) {
		tname = CCfgXMLConfig::GetAttr("key");
		if(tname.compare(tasksetname) == 0) {
			classifiername = CCfgXMLConfig::GetAttr("classifier");
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

	CCfgXMLConfig::Root()->Go("classifier")->Go(classifiername)->SetBranch();
	std::string description = CCfgXMLConfig::Branch()->QuickString("description");
	std::string valuetype = CCfgXMLConfig::Branch()->QuickString("icvalue");
	std::string labeltype = CCfgXMLConfig::Branch()->QuickString("iclabel");
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

	CCfgTasksetIter it = taskset->tasks.begin();
	while(it != taskset->tasks.end()) {
		ICClass* cclass = new ICClass(it->first, 0.00f);
		classifier->classes.Add(cclass);
		it++;
	}

	icmessage->classifiers.Add(classifier);
}
