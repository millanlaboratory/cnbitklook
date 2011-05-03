/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

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

#ifndef CCOBJECT_CPP
#define CCOBJECT_CPP

#include "CcObject.hpp"
#include "CcTime.hpp"
#include <cstdio>
#include <cstring>
#include <iomanip>

unsigned int CcObject::_objectInstances = 0;

CcObject::CcObject(const char* name, bool verbose) {
	this->_objectId = CcObject::_objectInstances++;
	this->_objectName.assign(name);
	CcTime::Datetime(&this->_objectDatetime);
	CcTime::Timestamp(&this->_objectTimestamp);
	this->_core = CcCore::Instance();
}

CcObject::~CcObject(void) {
	this->_core->Release();
	--CcObject::_objectInstances;
}

std::string CcObject::GetName(void) const {
	return this->_objectName;
}

CcObject* CcObject::SetName(std::string name) {
	this->_objectName.assign(name);
	return this;
}

unsigned int CcObject::GetInstances(void) const {
	return CcObject::_objectInstances;
}
	
const void CcObject::DumpInfo(void) const {
	printf("+ CcObject info:\n");
	printf("|-- Name:         %s\n", this->_objectName.c_str());
	printf("|-- Instance Id:  %d\n", this->_objectId);
	printf("|-- Instances:    %d\n", CcObject::_objectInstances);
	printf("|-- Pointer:      %p\n", (void*)this);
	printf("|-- Timestamp:    %s\n", this->_objectTimestamp.c_str());
	printf("`-- Datetime:     %s\n", this->_objectDatetime.c_str());
}

std::ostream &operator<<(std::ostream& output, const CcObject& obj) {
	output << "[" << obj.GetName() << "] ";
	return output;
}

std::ostream &operator<<(std::ostream& output, const CcObject* obj) {
	output << "[" << obj->GetName() << "] ";
	return output;
}
	
bool CcObject::operator==(const CcObject& right) {
	return(this->GetName().compare(right.GetName()) == 0);
}

bool CcObject::operator!=(const CcObject& right) {
	return(this->GetName().compare(right.GetName()) == 0);
}

#endif
