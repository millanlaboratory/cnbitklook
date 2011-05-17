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

#include "CCfgTaskset.hpp"
#include "XMLException.hpp"
#include "XMLTools.hpp"
#include <stdio.h>

CCfgTaskset::CCfgTaskset(const std::string& name) {
	this->name.assign(name);
	this->description.assign("unknown");
	//this->classifier.assign("unknown");
}

CCfgTask* CCfgTaskset::AddTask(CCfgTask* task) {
	CCfgTasksetIt it = this->tasks.find(task->name);
	if(it != this->tasks.end())
		return NULL;

	this->tasks[task->name] = task;
	return task;
}

CCfgTask* CCfgTaskset::RemoveTaskEx(const std::string& name) {
	CCfgTasksetIt it = this->tasks.find(name);
	if(it == this->tasks.end()) {
		std::string info;
		info.append("CCfgTask not found: ");
		info.append(name);
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	CCfgTask* retval = (*it).second;
	this->tasks.erase(it);
	return retval;
}

CCfgTask* CCfgTaskset::GetTaskEx(const std::string& name) {
	CCfgTasksetIt it = this->tasks.find(name);
	if(it == this->tasks.end()) {
		std::string info;
		info.append("CCfgTask not found: ");
		info.append(name);
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	CCfgTask* retval = (*it).second;
	return retval;
}

CCfgTask* CCfgTaskset::GetTaskEx(unsigned int id) {
	CCfgTasksetIt it;
	for(it = this->tasks.begin(); it != this->tasks.end(); it++) {
		if((*it).second->id == id)
			return (*it).second;
	}
	std::string info;
	info.append("CCfgTask not found: ");
	info.append(Tools::itos(id));
	throw XMLException(info, __PRETTY_FUNCTION__);
}

bool CCfgTaskset::HasTask(std::string name) {
	CCfgTasksetIt it = this->tasks.find(name);
	return(it != this->tasks.end());
}
		
unsigned int CCfgTaskset::Count(void) {
	return this->tasks.size();
}

void CCfgTaskset::Clear(void) {
	this->tasks.clear();
	this->description.assign("");
}

CCfgTasksetIt CCfgTaskset::Begin(void) {
	CCfgTasksetIt it = this->tasks.begin();
	return it;
}

CCfgTasksetIt CCfgTaskset::End(void) {
	CCfgTasksetIt it = this->tasks.end();
	return it;
}

CCfgTasksetConstIt CCfgTaskset::Begin(void) const {
	CCfgTasksetConstIt it = this->tasks.begin();
	return it;
}

CCfgTasksetConstIt CCfgTaskset::End(void) const {
	CCfgTasksetConstIt it = this->tasks.end();
	return it;
}

void CCfgTaskset::DumpNDF(void) {
	printf("NDF config for taskset %s\n", this->name.c_str());
	printf(" Exec:         %s\n", this->ndf.exec.c_str());
	printf(" Pipename:     %s\n", this->ndf.pipe.c_str());
	printf(" iD:           %s\n", this->ndf.id.c_str());
	printf(" iC:           %s\n", this->ndf.ic.c_str());
	printf(" Directory:    %s\n", this->ndf.directory.c_str());
	printf(" Include:      %s\n", this->ndf.include.c_str());
}

void CCfgTaskset::DumpClassifier(void) {
	printf("Classifier config for taskset %s\n", this->name.c_str());
	printf(" Name:         %s\n", this->classifier.name.c_str());
	printf(" Description:  %s\n", this->classifier.description.c_str());
	printf(" Filename:     %s\n", this->classifier.filename.c_str());
}
