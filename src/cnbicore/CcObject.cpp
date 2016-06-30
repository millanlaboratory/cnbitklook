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

#ifndef CCOBJECT_CPP
#define CCOBJECT_CPP

#include "CcObject.hpp"
#include <cstdio>

unsigned int CcObject::_objectInstances = 0;

CcObject::CcObject(void) {
	this->_objectId = CcObject::_objectInstances++;
	this->_core = CcCore::Instance();
}

CcObject::~CcObject(void) {
	this->_core->Release();
	--CcObject::_objectInstances;
}

unsigned int CcObject::GetInstances(void) const {
	return CcObject::_objectInstances;
}
	
const void CcObject::DumpInfo(void) const {
	printf("+ CcObject info:\n");
	printf("|-- Instance Id:  %d\n", this->_objectId);
	printf("|-- Instances:    %d\n", CcObject::_objectInstances);
	printf("`-- Pointer:      %p\n", (void*)this);
}

#endif
