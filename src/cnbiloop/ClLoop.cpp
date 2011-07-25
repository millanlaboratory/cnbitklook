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
#include "ClLoopConfig.hpp" 
#include <cnbicore/CcSocket.hpp>

/* Initialization */
ClLoop* ClLoop::_instance = NULL;
unsigned int ClLoop::_refCount = 0;

/* Declarations */	
ClProClient ClLoop::pro;
ClAcqClient ClLoop::acq;
ClNmsClient ClLoop::nms;
CcAddress ClLoop::_addrPro;
CcAddress ClLoop::_addrAcq;
CcAddress ClLoop::_addrNms;

ClLoop::ClLoop(void) {
}

ClLoop::~ClLoop(void) {
	ClLoop::Disconnect();
}

ClLoop* ClLoop::Instance(void) {
	if(ClLoop::_instance == NULL)
		ClLoop::_instance = new ClLoop;
	++ClLoop::_refCount;
	ClLoopConfig::Instance();
	return ClLoop::_instance;
}

unsigned int ClLoop::Refcount(void) {
	return ClLoop::_refCount;
}

void ClLoop::Release(void) {
	if(--ClLoop::_refCount < 1) 
		ClLoop::Destroy();
	ClLoopConfig::Release();
}

void ClLoop::Destroy(void) {
	if(ClLoop::_instance == NULL) 
		return;
	delete ClLoop::_instance;
}

bool ClLoop::Connect(void) {
	CcAddress nameserver;
	
	/*
	CcAddress envvar = CcCore::GetEnvCnbiTkAddress();
	if(envvar.empty() == false) {
		CcLogDebugS("$CNBITK_ADDRESS defined as " << envvar);
		nameserver = CcSocket::Lookup(envvar) + ":8123";
	} else {
		CcLogDebug("$CNBITK_ADDRESS not defined, assuming localhost");
		nameserver = "127.0.0.1:8123";
	}

	int status = ClLoop::Connect(nameserver);
	CcLogDebugS("CnbiTk loop nameserver: " << ClLoop::_addrNms);
	CcLogDebugS("CnbiTk loop pro: " << ClLoop::_addrPro);
	CcLogDebugS("CnbiTk loop acq: " << ClLoop::_addrAcq);
	return status; 
	*/
	return 1;
}
		
bool ClLoop::Connect(CcAddress nameserver) {
	ClLoop::_addrNms = nameserver;
	if(ClLoop::ConnectNms() == false)
		return false;
	if(ClLoop::QueryAddresses() == false)
		return false;
	if(ClLoop::ConnectPro() == false)
		return false;
	if(ClLoop::ConnectAcq() == false)
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
	ClLoop::pro.Disconnect();
	ClLoop::acq.Disconnect();
	ClLoop::nms.Disconnect();
}
		
bool ClLoop::IsConnected(void) {
	return ClLoop::pro.IsConnected() &&
		ClLoop::acq.IsConnected() &&
		ClLoop::nms.IsConnected();
}

bool ClLoop::ConnectNms(void) {
	if(ClLoop::nms.Connect(ClLoop::_addrNms)) 
		return true;

	CcLogDebugS("Cannot connect to nameserver: " << ClLoop::_addrNms);
	return false;
}

bool ClLoop::ConnectPro(void) {
	if(ClLoop::pro.Connect(ClLoop::_addrPro)) 
		return true;

	CcLogDebugS("Cannot connect to pro: " << ClLoop::_addrPro);
	return false;
}

bool ClLoop::ConnectAcq(void) {
	if(ClLoop::acq.Connect(ClLoop::_addrAcq)) 
		return true;

	CcLogDebugS("Cannot connect to acq: " << ClLoop::_addrAcq);
	return false;
}

bool ClLoop::QueryAddresses(void) {
	int sp = ClLoop::nms.Query("/pro", &ClLoop::_addrPro);
	int sa = ClLoop::nms.Query("/acq", &ClLoop::_addrAcq);

	if(sp != ClNmsLang::Successful)
		return false;
	if(sa != ClNmsLang::Successful)
		return false;

	return true;
}

#endif
