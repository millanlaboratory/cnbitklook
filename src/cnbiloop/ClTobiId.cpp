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

ClTobiId::ClTobiId(int mode) {
	ClLoop::Instance();
	this->_client = NULL;
	this->_mode = mode;
}

ClTobiId::~ClTobiId(void) {
	ClLoop::Release();
}

bool ClTobiId::Attach(const std::string& name) {
	if(this->_client != NULL)
		return false;
	
	this->_name.assign(name);

	if(ClLoop::Connect() == false) { 
		CcLogDebug("Cannot connect to loop");
		return false;
	}

	CcAddress address;
	if(ClLoop::nameserver.Query(this->_name, &address) != ClNamesLang::Successful) {
		CcLogDebugS("Query returned empty result: " << this->_name);
		return false;
	}

	CcEndpoint endpoint(address);
	this->_client = new CcClient();
	try {
		this->_client->Connect(endpoint.GetPort());
	} catch(CcException e) {
		CcLogDebugS("Cannot connect to " << address);
		return false;
	}
	
	CB_CcSocket(this->_client->iOnConnect, this, HandleConnect);
	CB_CcSocket(this->_client->iOnDisconnect, this, HandleDisconnect);
	CB_CcSocket(this->_client->iOnRecv, this, HandleRecv);

	/* 2011-05-12  Michele Tavella <michele.tavella@epfl.ch>
	 * Found delay in starting iD communication. Must be inspected.
	 * This is a temporary patch/hack.
	 */
	//CcTime::Sleep(5000);

	//for(int i = 0; i < 50; i++) {
	//	//CcLogFatalS("Connected: " << this->_client->IsConnected());
	//	CcLogFatalS("Connected: " << this->_client->IsConnected() << " " <<
	//		this->_client->Send("ciao"));
	//	CcTime::Sleep(250);
	//}

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
	CcLogDebugS("Connected TCP endpoint: " << client->GetRemote());
}

void ClTobiId::HandleDisconnect(CcSocket* caller) {
	CcClient* client = (CcClient*)caller;
	CcLogDebugS("Disconnected TCP endpoint: " << client->GetRemote());
}

void ClTobiId::HandleRecv(CcSocket* caller, CcStreamer* stream) {
	this->_semqueue.Wait();
	
	if(this->_mode == ClTobiId::SetOnly) {
		stream->Clear();	
	} else {
		std::string buffer;
		if(stream->Extract(&buffer, "<tobiid", "/>") == true) 
			this->_queue.push_back(buffer);
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
		
bool ClTobiId::SetMessage(IDSerializerRapid* serializer, int blockidx) {
	if(this->_mode == ClTobiId::GetOnly) {
		CcLogError("iD interface configures as GetOnly");
		return false;
	}
	
	std::string buffer;
	serializer->message->absolute.Tic();
	serializer->message->SetBlockIdx(blockidx);
	serializer->Serialize(&buffer);
	return(this->_client->Send(buffer) == (int)buffer.size());
}

#endif
