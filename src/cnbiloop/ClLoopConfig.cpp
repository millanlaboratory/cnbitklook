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

#ifndef CLLOOPCONFIG_CPP 
#define CLLOOPCONFIG_CPP 

#include "ClLoopConfig.hpp" 

/* Initialization */
ClLoopConfig* ClLoopConfig::_instance = NULL;
unsigned int ClLoopConfig::_refCount = 0;

ClLoopConfig::ClLoopConfig(void) {
}

ClLoopConfig::~ClLoopConfig(void) {
}

ClLoopConfig* ClLoopConfig::Instance(void) {
	if(ClLoopConfig::_instance == NULL)
		ClLoopConfig::_instance = new ClLoopConfig;
	++ClLoopConfig::_refCount;
	return ClLoopConfig::_instance;
}

unsigned int ClLoopConfig::Refcount(void) {
	return ClLoopConfig::_refCount;
}

void ClLoopConfig::Release(void) {
	if(--ClLoopConfig::_refCount < 1) 
		ClLoopConfig::Destroy();
}

void ClLoopConfig::Destroy(void) {
	if(ClLoopConfig::_instance == NULL) 
		return;
	delete ClLoopConfig::_instance;
}

#endif
