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

#ifndef CLTOBIID_CPP 
#define CLTOBIID_CPP 

#include "ClTobiId.hpp" 

ClTobiId::ClTobiId(int mode, float waitms) {
	ClLoop::Instance();
	this->_client = NULL;
	this->_mode = mode;
	this->_waitms = waitms;
}

ClTobiId::~ClTobiId(void) {
	ClLoop::Release();
}

bool ClTobiId::Attach(const std::string& name) {
	if(this->_client != NULL)
		return false;
	
	this->_name.assign(name);
	
	this->_client = new CcClientExt;
	CB_CcSocket(this->_client->iOnConnect, this, HandleConnect);
	CB_CcSocket(this->_client->iOnDisconnect, this, HandleDisconnect);
	CB_CcSocket(this->_client->iOnRecv, this, HandleRecv);

	if(ClLoop::Connect() == false) { 
		CcLogError("Cannot connect to loop");
		return false;
	}

	CcAddress address;
	if(ClLoop::nms.Query(this->_name, &address) != ClNmsLang::Successful) {
		CcLogErrorS("Cannot query " << this->_name);
		return false;
	}

	if(this->_client->Connect(address) == false) {
		CcLogErrorS("Cannot connect to " << address);
		return false;
	}
	

	return true;
}

bool ClTobiId::Detach(void) {
	if(this->_client == NULL)
		return true;

	if(this->_client->IsConnected())
		this->_client->Disconnect();
	delete(this->_client);
	this->_client = NULL;
	return true;
}

bool ClTobiId::IsAttached(void) {
	if(this->_client == NULL)
		return false;
	return this->_client->IsConnected();
}

void ClTobiId::HandleConnect(CcSocket* caller) {
	CcClientExt* client = (CcClientExt*)caller;
	CcLogDebugS("Connected TCP endpoint: " << client->GetRemote());
	this->iOnAttach.Execute();
}

void ClTobiId::HandleDisconnect(CcSocket* caller) {
	CcClientExt* client = (CcClientExt*)caller;
	CcLogDebugS("Disconnected TCP endpoint: " << client->GetRemote());
	this->iOnDetach.Execute();
}

void ClTobiId::HandleRecv(CcSocket* caller, CcStreamer* stream) {
	this->_semqueue.Wait();
	
	//if(this->_mode == ClTobiId::SetOnly) {
	//	stream->Clear();	
	//} else {
	if(this->_mode != ClTobiId::SetOnly) {
		std::string buffer;
		while(stream->Extract(&buffer, "<tobiid", "/>") == true) {
			this->_queue.push_back(buffer);
			this->iOnHasMessage.Execute();
		}
	}
	this->_semqueue.Post();
}
		
bool ClTobiId::GetMessage(IDSerializerRapid* serializer) {
	if(this->_mode == ClTobiId::SetOnly) {
		CcLogError("iD interface configures as SetOnly");
		return false;
	}

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
		
int ClTobiId::Clear(void) {
	int count = 0;
	this->_semqueue.Wait();
	count = this->_queue.size();
	this->_queue.clear();
	this->_semqueue.Post();
	return count;
}
		
bool ClTobiId::SetMessage(IDSerializerRapid* serializer, int blockidx, int* fidx) {
	if(this->_mode == ClTobiId::GetOnly) {
		CcLogError("iD interface configures as GetOnly");
		return false;
	}
	
	// Assemble iD message and serialize it
	std::string buffer;
	serializer->message->absolute.Tic();
	serializer->message->SetBlockIdx(blockidx);
	serializer->Serialize(&buffer);
	
	// Prepare stuff for TCLanguage
	std::string reply;
	int component, status, tcfidx;

	if(!this->_client->SendRecv(buffer, &reply, "<tcstatus", "/>", this->_waitms)) {
		CcLogWarningS("TiD status message not received after " << this->_waitms << "ms");
		return false;
	}

	if(!this->_tclang.IsStatus(reply, &component, &status, &tcfidx)) {
		CcLogWarningS("TiD status message received but is not valid: " << reply);
		return false;
	}
	
	if(status != TCLanguage::Ready) {
		CcLogErrorS("TiD interface reports to be down");
		return false;
	}
	
	if(fidx != NULL) 
		*fidx = tcfidx;
	return true;
}

// Created by L.Tonin  <luca.tonin@epfl.ch> on 09/06/17 10:55:07
int ClTobiId::GetMode(void) {
	return this->_mode;
}

#endif
