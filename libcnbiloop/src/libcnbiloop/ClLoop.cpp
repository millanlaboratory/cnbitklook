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
	this->Disconnect();
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
		
void ClLoop::Disconnect(void) {
	this->processing.Disconnect();
	this->acquisition.Disconnect();
	this->nameserver.Disconnect();
}
		
bool ClLoop::IsConnected(void) {
	return this->processing.IsConnected() &&
		this->acquisition.IsConnected() &&
		this->nameserver.IsConnected();
}

bool ClLoop::ConnectNameserver(void) {
	int status = this->nameserver.Connect(this->_nameserver);
	if(status != ClNamesLang::Successful) {
		CcLogErrorS(this->_stream, "Cannot connect to nameserver: " <<
				this->_nameserver << ", " << status);
		return false;
	}
	return true;
}

bool ClLoop::ConnectProcessing(void) {
	int status = this->processing.Connect(this->_processing);
	if(status != ClProLang::Successful) {
		CcLogErrorS(this->_stream, "Cannot connect to processing: " <<
				this->_processing << ", " << status);
		return false;
	}
	return true;
}

bool ClLoop::ConnectAcquisition(void) {
	int status = this->acquisition.Connect(this->_acquisition);
	if(status != ClAcqLang::Successful) {
		CcLogErrorS(this->_stream, "Cannot connect to acquisition: " <<
				this->_acquisition << ", " << status);
		return false;
	}
	return true;
}

bool ClLoop::QueryAddresses(void) {
	int sp = this->nameserver.Query("/processing", &this->_processing);
	int sa = this->nameserver.Query("/acquisition", &this->_acquisition);

	if(sp != ClNamesLang::Successful)
		return false;
	if(sa != ClNamesLang::Successful)
		return false;

	return true;
}

#endif
