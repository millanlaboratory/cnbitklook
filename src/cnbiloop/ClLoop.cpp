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

#ifndef CLLOOP_CPP 
#define CLLOOP_CPP 

#include "ClLoop.hpp" 
#include <cnbicore/CcSocket.hpp>

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
		
const CcPort ClLoop::portNsr = "8123";
const CcPort ClLoop::portPro = "8124";
const CcPort ClLoop::portAcq = "8125";
const CcPort ClLoop::portBus = "8126";
const CcPort ClLoop::portDev = "8127";

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

bool ClLoop::Connect(void) {
	CcAddress nameserver;
	CcAddress envvar = CcCore::GetEnvCnbiTkAddress();
	if(envvar.empty() == false) {
		CcLogDebugS("$CNBITK_ADDRESS defined as " << envvar);
		nameserver = CcSocket::Lookup(envvar) + ":8123";
	} else {
		CcLogDebug("$CNBITK_ADDRESS not defined, assuming localhost");
		nameserver = "127.0.0.1:8123";
	}

	int status = ClLoop::Connect(nameserver);
	CcLogDebugS("CnbiTk loop nameserver: " << ClLoop::_nameserver);
	CcLogDebugS("CnbiTk loop processing: " << ClLoop::_processing);
	CcLogDebugS("CnbiTk loop acquisition: " << ClLoop::_acquisition);
	return status; 
}
		
bool ClLoop::Connect(CcAddress nameserver) {
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
		
bool ClLoop::Connect(CCfgConfig* configuration) {
	CcAddress nameserver;
	try {
	nameserver =
		configuration->RootEx()->QuickStringEx("configuration/clloop/nameserver");
	} catch(CcException e) {
		CcLogError("Configuration failed, using default values");
		return ClLoop::Connect();
	}
	return ClLoop::Connect(nameserver);
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
	if(ClLoop::nameserver.Connect(ClLoop::_nameserver)) 
		return true;

	CcLogDebugS("Cannot connect to nameserver: " << ClLoop::_nameserver);
	return false;
}

bool ClLoop::ConnectProcessing(void) {
	if(ClLoop::processing.Connect(ClLoop::_processing)) 
		return true;

	CcLogDebugS("Cannot connect to processing: " << ClLoop::_processing);
	return false;
}

bool ClLoop::ConnectAcquisition(void) {
	if(ClLoop::acquisition.Connect(ClLoop::_acquisition)) 
		return true;

	CcLogDebugS("Cannot connect to acquisition: " << ClLoop::_acquisition);
	return false;
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
