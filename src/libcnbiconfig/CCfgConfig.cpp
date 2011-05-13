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

#include "CCfgConfig.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
		
bool CCfgConfig::Validate(void) {
	try {
		// Test main nodes
		CCfgXMLConfig::RootEx()->GoEx("subject");
		CCfgXMLConfig::RootEx()->GoEx("recording");
		CCfgXMLConfig::RootEx()->GoEx("protocol");
		CCfgXMLConfig::RootEx()->GoEx("options");
		CCfgXMLConfig::RootEx()->GoEx("offline");
		CCfgXMLConfig::RootEx()->GoEx("online");
		CCfgXMLConfig::RootEx()->GoEx("classifier");
		CCfgXMLConfig::RootEx()->GoEx("classifiers");
		CCfgXMLConfig::RootEx()->GoEx("tasksets");
		CCfgXMLConfig::RootEx()->GoEx("tasks");
		CCfgXMLConfig::RootEx()->GoEx("events");
		CCfgXMLConfig::RootEx()->GoEx("configuration");
		
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
	CCfgXMLConfig::RootEx()->GoEx("tasksets")->GoEx(name)->SetBranch();
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
		task->gdf = converter.GDFInt();
		
		// Finally add task to taskset
		taskset->AddTask(task);

		// Loop for next
		node = CCfgXMLConfig::NextSibling();
	}
}


void CCfgConfig::ParseConfigEx(const std::string& mode, const std::string& block,
		const std::string& tasksetname, CCfgTaskset* taskset) {
	CCfgXMLConfig::RootEx()->GoEx(mode)->GoEx(block)->GoEx("taskset")->SetBranch();
	XMLNode node = CCfgXMLConfig::_nBranch;
	
	std::string tname;
	while(node != NULL) {
		tname = CCfgXMLConfig::GetAttrEx("ttype");
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
		info += block;
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
		
void CCfgConfig::ParseClassifierEx(const std::string& bl, const std::string& ts,
		CCfgTaskset* taskset, ICMessage* icmessage) {
	CCfgXMLConfig::RootEx()->GoEx("online")->GoEx(bl)->GoEx("taskset")->SetBranch();
	
	/* Among the tasksets, find the one named ts 
	 * and make sure tnode points to it
	 */
	std::string tname, cname, ctype;
	XMLNode tnode = CCfgXMLConfig::_nBranch;
	while(tnode != NULL) {
		tname = CCfgXMLConfig::GetAttrEx("ttype");
		if(tname.compare(ts) == 0) {
			cname = CCfgXMLConfig::GetAttrEx("classifier");
			ctype = CCfgXMLConfig::GetAttrEx("ctype");
			//taskset->classifier.assign(cname);
			tnode = CCfgXMLConfig::Child();
			break;
		}
		tnode = CCfgXMLConfig::NextSibling();
	}

	/* If tnode points to NULL, it means the taskset ts
	 * was not found within block bl
	 */
	if(tnode == NULL) {
		std::string info;
		info += "Block ";
		info += bl;
		info += " does not contain taskset ";
		info += ts;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	/* We need to use cname to get the description and filename for the
	 * classifier cname
	 */
	CCfgXMLConfig::RootEx()->GoEx("classifier")->GoEx(cname)->SetBranch();

	std::string cdesc, cfile;
	cdesc = CCfgXMLConfig::BranchEx()->QuickStringEx("description");
	cfile = CCfgXMLConfig::BranchEx()->QuickStringEx("filename");

	taskset->classifier.name = cname;
	taskset->classifier.description = cdesc;
	taskset->classifier.filename = cfile;
	
	/* Configure ICMessage icmessage
	 * - we need to use ctype to find the right classifier type in
	 *   cnbiconfig/classifiers
	 * - then we need to get all the IC and NDF parameters
	 */
	CCfgXMLConfig::RootEx()->GoEx("classifiers")->GoEx(ctype)->SetBranch();

	taskset->ndf.function =
		CCfgXMLConfig::BranchEx()->GoEx("ndf")->QuickStringEx("function");
	taskset->ndf.pipename =
		CCfgXMLConfig::BranchEx()->GoEx("ndf")->QuickStringEx("pipename");
	taskset->ndf.id = 
		CCfgXMLConfig::BranchEx()->GoEx("ndf")->QuickStringEx("id");
	taskset->ndf.ic = 
		CCfgXMLConfig::BranchEx()->GoEx("ndf")->QuickStringEx("ic");

	if(icmessage == NULL)
		return;

	/* Configure ICMessage icmessage
	 * - we need to use ctype to find the right classifier type in
	 *   cnbiconfig/classifiers
	 * - then we need to get all the IC and NDF parameters
	 */
	std::string vtype, ltype;
	vtype = CCfgXMLConfig::BranchEx()->GoEx("tobi")->QuickStringEx("icvalue");
	ltype = CCfgXMLConfig::BranchEx()->GoEx("tobi")->QuickStringEx("iclabel");
	
	ICClassifier* classifier = new ICClassifier(cname, cdesc);
	if(classifier->SetValueType(vtype) == false) {
		std::string info;
		info += "ICVtype not known: ";
		info += vtype;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	if(classifier->SetLabelType(ltype) == false) {
		std::string info;
		info += "ICLtype not known: ";
		info += ltype;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	CCfgTasksetIt it = taskset->tasks.begin();
	while(it != taskset->tasks.end()) {
		std::string label;
		
		ICClass* cclass = NULL;
		if(ltype.compare(ICClassifier::TxtLabelBiosig) == 0) {
			//char cache[16];
			//sprintf(cache, "0x%X", it->second->gdf);
			//label.assign(cache);
			cclass = new ICClass(it->second->gdf, 0.00f);
		} else {
			//label.assign(it->first);
			cclass = new ICClass(it->first, 0.00f);
		}
		//ICClass* cclass = new ICClass(label, 0.00f);
		classifier->classes.Add(cclass);
		it++;
	}
	icmessage->classifiers.Add(classifier);
}
