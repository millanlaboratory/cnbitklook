/*
    Copyright (C) 2011 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLLOOP_CPP 
#define CLLOOP_CPP 

#include "ClLoop.hpp" 

ClLoop::ClLoop(void) {
}

ClLoop::~ClLoop(void) {
}
		
bool ClLoop::Connect(CcAddress nameserver) {
	this->_nameserver = nameserver;
	if(this->ConnectNameserver() == false)
		return false;

	if(this->QueryAddresses() == false)
		return false;

	if(this->ConnectProcessing() == false)
		return false;
	if(this->ConnectAcquisition() == false)
		return false;

	return true;
}
		
bool ClLoop::Disconnect(void) {
}

bool ClLoop::ConnectNameserver(void) {
	return this->nameserver.Connect(this->_nameserver);
}

bool ClLoop::ConnectProcessing(void) {
	return this->processing.Connect(this->_processing);
}

bool ClLoop::ConnectAcquisition(void) {
	return this->acquisiton.Connect(this->_acquisiton);
}

bool ClLoop::QueryAddresses(void) {
	int sp = this->nameserver.Query("/processing", &this->_processing);
	int sa = this->nameserver.Query("/acquisiton", &this->_acquisiton);

	if(sp != ClNamesLang::Successful)
		return false;
	if(sa != ClNamesLang::Successful)
		return false;

	return true;
}

#endif
