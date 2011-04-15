/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

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

#ifndef CLTOBIID_CPP 
#define CLTOBIID_CPP 

#include "ClTobiId.hpp" 

ClTobiId::ClTobiId(void) {
	ClLoop::Instance();
	this->_client = NULL;
}

ClTobiId::~ClTobiId(void) {
	ClLoop::Release();
}

bool ClTobiId::Attach(const std::string& name) {
	if(this->_client != NULL)
		return false;
	
	this->_name.assign(name);

	if(ClLoop::Connect() == false) { 
		CcLogError("Cannot connect to loop");
		return false;
	}

	CcAddress address;
	if(ClLoop::nameserver.Query(this->_name, &address) != ClNamesLang::Successful) {
		CcLogErrorS(this->_stream, "Query returned empty result: " << this->_name);
		return false;
	}

	CcEndpoint endpoint(address);
	this->_client = new CcClient();
	try {
		this->_client->Connect(endpoint);
	} catch(CcException e) {
		CcLogErrorS(this->_stream, "Cannot connect to " << address);
		return false;
	}
	
	CB_CcSocket(this->_client->iOnConnect, this, HandleConnect);
	CB_CcSocket(this->_client->iOnDisconnect, this, HandleDisconnect);
	CB_CcSocket(this->_client->iOnRecv, this, HandleRecv);

	return true;
}

bool ClTobiId::Detach(void) {
	if(this->_client == NULL)
		return true;

	this->_client->Disconnect();
	delete(this->_client);
	this->_client = NULL;
	return true;
}

bool ClTobiId::IsAttached(void) {
	return this->_client->IsConnected();
}

void ClTobiId::HandleConnect(CcSocket* caller) {
	CcClient* client = (CcClient*)caller;
	CcLogDebugS(this->_stream, "Connected TCP endpoint: " << 
			client->GetRemote());
}

void ClTobiId::HandleDisconnect(CcSocket* caller) {
	CcClient* client = (CcClient*)caller;
	CcLogDebugS(this->_stream, "Disconnected TCP endpoint: " << 
			client->GetRemote());
}

void ClTobiId::HandleRecv(CcSocket* caller) {
	if(this->_semqueue.TryWait() == false) 
		return;
	
	std::string buffer;
	bool status = this->_client->datastream.Extract(&buffer, "<tobiid",
			"/>");
	if(status) 
		this->_queue.push_back(buffer);
	this->_semqueue.Post();
}
		
bool ClTobiId::GetMessage(IDSerializerRapid* serializer) {
	this->_semqueue.Wait();
	if(this->_queue.empty()) {
		this->_semqueue.Post();
		return false;
	}
	std::string buffer = this->_queue.front();
	serializer->Deserialize(&buffer);
	this->_queue.pop_front();
	this->_semqueue.Post();
	return true;
}
		
int ClTobiId::Count(void) {
	int count = 0;
	this->_semqueue.Wait();
	count = this->_queue.size();
	this->_semqueue.Post();
	return count;
}

#endif
