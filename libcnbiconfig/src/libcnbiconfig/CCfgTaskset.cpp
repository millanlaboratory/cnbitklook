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

#include "CCfgTaskset.hpp"
#include "XMLException.hpp"
#include "Tools.hpp"
		
CCfgTask* CCfgTaskset::Add(CCfgTask* task) {
	CCfgTasksetIter it = this->tasks.find(task->name);
	if(it != this->tasks.end())
		return NULL;

	this->tasks[task->name] = task;
	return task;
}

CCfgTask* CCfgTaskset::Remove(std::string name) {
	CCfgTasksetIter it = this->tasks.find(name);
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

CCfgTask* CCfgTaskset::Get(std::string name) {
	CCfgTasksetIter it = this->tasks.find(name);
	if(it == this->tasks.end()) {
		std::string info;
		info.append("CCfgTask not found: ");
		info.append(name);
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	CCfgTask* retval = (*it).second;
	return retval;
}

#include <iostream>
CCfgTask* CCfgTaskset::Get(unsigned int id) {
	CCfgTasksetIter it;
	for(it = this->tasks.begin(); it != this->tasks.end(); it++) {
		if((*it).second->id == id)
			return (*it).second;
	}
	std::string info;
	info.append("CCfgTask not found: ");
	info.append(Tools::itos(id));
	throw XMLException(info, __PRETTY_FUNCTION__);
}

bool CCfgTaskset::Has(std::string name) {
	CCfgTasksetIter it = this->tasks.find(name);
	return(it != this->tasks.end());
}
		
unsigned int CCfgTaskset::Count(void) {
	return this->tasks.size();
}

void CCfgTaskset::Clear(void) {
	this->tasks.clear();
	this->description.assign("");
}

CCfgTasksetIter CCfgTaskset::Begin(void) {
	CCfgTasksetIter it = this->tasks.begin();
	return it;
}

CCfgTasksetIter CCfgTaskset::End(void) {
	CCfgTasksetIter it = this->tasks.end();
	return it;
}

CCfgTasksetConstIter CCfgTaskset::Begin(void) const {
	CCfgTasksetConstIter it = this->tasks.begin();
	return it;
}

CCfgTasksetConstIter CCfgTaskset::End(void) const {
	CCfgTasksetConstIter it = this->tasks.end();
	return it;
}
