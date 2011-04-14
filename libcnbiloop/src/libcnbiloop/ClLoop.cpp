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
/* Initialization */
ClLoop* ClLoop::_instance = NULL;
unsigned int ClLoop::_refCount = 0;

/* Declarations */	
ClProClient ClLoop::processing;
ClAcqClient ClLoop::acquisition;
ClNamesClient ClLoop::nameserver;
CcAddress ClLoop::_processing;
CcAddress ClLoop::_acquisition;
CcAddress ClLoop::_nameserver;
std::stringstream ClLoop::_stream;

ClLoop::ClLoop(void) {
}

ClLoop::~ClLoop(void) {
	ClLoop::Disconnect();
}
		
ClLoop* ClLoop::Instance(void) {
	if(ClLoop::_instance == NULL)
		ClLoop::_instance = new ClLoop;
	++ClLoop::_refCount;
	return ClLoop::_instance;
}

unsigned int ClLoop::Refcount(void) {
	return ClLoop::_refCount;
}

void ClLoop::Release(void) {
	if(--ClLoop::_refCount < 1) 
		ClLoop::Destroy();
}

void ClLoop::Destroy(void) {
	if(ClLoop::_instance == NULL) 
		return;
	delete ClLoop::_instance;
}
		
bool ClLoop::Connect(CcAddress nameserver) {
	if(ClLoop::_refCount < 1)
		ClLoop::Instance();
	ClLoop::_nameserver = nameserver;
	
	if(ClLoop::ConnectNameserver() == false)
		return false;
	if(ClLoop::QueryAddresses() == false)
		return false;
	if(ClLoop::ConnectProcessing() == false)
		return false;
	if(ClLoop::ConnectAcquisition() == false)
		return false;

	return true;
}
		
void ClLoop::Disconnect(void) {
	ClLoop::processing.Disconnect();
	ClLoop::acquisition.Disconnect();
	ClLoop::nameserver.Disconnect();
}
		
bool ClLoop::IsConnected(void) {
	return ClLoop::processing.IsConnected() &&
		ClLoop::acquisition.IsConnected() &&
		ClLoop::nameserver.IsConnected();
}

bool ClLoop::ConnectNameserver(void) {
	int status = ClLoop::nameserver.Connect(ClLoop::_nameserver);
	if(status != ClNamesLang::Successful) {
		CcLogErrorS(ClLoop::_stream, "Cannot connect to nameserver: " <<
				ClLoop::_nameserver << ", " << status);
		return false;
	}
	return true;
}

bool ClLoop::ConnectProcessing(void) {
	int status = ClLoop::processing.Connect(ClLoop::_processing);
	if(status != ClProLang::Successful) {
		CcLogErrorS(ClLoop::_stream, "Cannot connect to processing: " <<
				ClLoop::_processing << ", " << status);
		return false;
	}
	return true;
}

bool ClLoop::ConnectAcquisition(void) {
	int status = ClLoop::acquisition.Connect(ClLoop::_acquisition);
	if(status != ClAcqLang::Successful) {
		CcLogErrorS(ClLoop::_stream, "Cannot connect to acquisition: " <<
				ClLoop::_acquisition << ", " << status);
		return false;
	}
	return true;
}

bool ClLoop::QueryAddresses(void) {
	int sp = ClLoop::nameserver.Query("/processing", &ClLoop::_processing);
	int sa = ClLoop::nameserver.Query("/acquisition", &ClLoop::_acquisition);

	if(sp != ClNamesLang::Successful)
		return false;
	if(sa != ClNamesLang::Successful)
		return false;

	return true;
}

#endif
