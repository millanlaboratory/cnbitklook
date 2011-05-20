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

#ifndef CLTOBIIC_CPP 
#define CLTOBIIC_CPP 

#include "ClTobiIc.hpp" 

ClTobiIc::ClTobiIc(void) {
	ClLoop::Instance();
	this->_server = NULL;
	this->_onwsname = false;
}

ClTobiIc::~ClTobiIc(void) {
	ClLoop::Release();
}

bool ClTobiIc::Attach(const std::string& name) {
	if(this->_server != NULL)
		return false;

	if(ClLoop::Connect() == false) { 
		CcLogErrorS("Cannot connect to loop");
		return false;
	}
	this->_name.assign(name);
	this->_onwsname = false;
	
	CcAddress address;
	int status = ClLoop::nameserver.Query(this->_name, &address);
	if(status != ClNamesLang::Successful) {
		CcLogErrorS("Cannot query " << name);
		return false;
	}
	
	CcEndpoint peer(address);
	this->_server = new CcServer();
	CB_CcSocket(this->_server->iOnAccept, this, HandleAccept);
	CB_CcSocket(this->_server->iOnDrop, this, HandleDrop);
	CB_CcSocket(this->_server->iOnRecvPeer, this, HandleRecvPeer);
	
	if(this->_server->Bind(peer.GetPort()) == false) {
		CcLogErrorS("Cannot bind to port " << peer.GetPort());
		return false;
	}

	this->_hasmessage.Wait();
	this->iOnAttach.Execute();
	return true;
}
		
bool ClTobiIc::Attach(const CcPort port, const std::string& name) {
	if(this->_server != NULL)
		return false;
	
	if(ClLoop::Connect() == false) { 
		CcLogErrorS("Cannot connect to loop");
		return false;
	}
	this->_name.assign(name);
	this->_onwsname = true;
	
	CcEndpoint peer("0.0.0.0", port);
	this->_server = new CcServer();
	CB_CcSocket(this->_server->iOnAccept, this, HandleAccept);
	CB_CcSocket(this->_server->iOnDrop, this, HandleDrop);
	CB_CcSocket(this->_server->iOnRecvPeer, this, HandleRecvPeer);

	if(this->_server->Bind(peer.GetPort()) == false) {
		CcLogErrorS("Cannot bind to port " << port);
		return false;
	}

	int status = ClLoop::nameserver.Set(this->_name, peer.GetAddress());
	if(status != ClNamesLang::Successful) {
		CcLogErrorS("Cannot set " << name << " as " << peer.GetAddress());
		return false;
	}

	this->_hasmessage.Wait();
	this->iOnAttach.Execute();
	return true;
}

bool ClTobiIc::Detach(void) {
	if(this->_server == NULL)
		return true;

	if(this->_onwsname == true)
		ClLoop::nameserver.Unset(this->_name);
	this->_server->Release();
	delete(this->_server);
	this->_server = NULL;
	this->_hasmessage.Post();
	
	this->iOnDetach.Execute();
	return true;
}

bool ClTobiIc::IsAttached(void) {
	return this->_server->IsConnected();
}

int ClTobiIc::WaitMessage(ICSerializerRapid* serializer) {
	if(this->IsAttached() == false)
		return ClTobiIc::Detached;
	if(this->_hasmessage.TryWait() == false)
		return ClTobiIc::NoMessage;
	return Deserialize(serializer);
}

int ClTobiIc::GetMessage(ICSerializerRapid* serializer) {
	if(this->IsAttached() == false)
		return ClTobiIc::Detached;
	this->_hasmessage.Wait();
	if(this->IsAttached() == false)
		return ClTobiIc::Detached;
	return Deserialize(serializer);
}

int ClTobiIc::Deserialize(ICSerializerRapid* serializer) {
	this->_sembuffer.Wait();
	try {
		serializer->Deserialize(&this->_buffer);
		this->_buffer.clear();
	} catch(TCException e) { 
		this->_sembuffer.Post();
		return ClTobiIc::NoMessage;
	}
	this->_sembuffer.Post();
	return ClTobiIc::HasMessage;
}

void ClTobiIc::HandleRecvPeer(CcSocket* caller, CcAddress addr, 
		CcStreamer* stream) { 
	if(this->_sembuffer.TryWait() == false) 
		return;

	if(stream->Extract(&this->_buffer, "<tobiic", "</tobiic>") == true) {
		this->_hasmessage.Post();
	}
	this->_sembuffer.Post();
	this->iOnHasMessage.Execute();
}

void ClTobiIc::HandleAccept(CcSocket* caller) { 
	CcServer *server = (CcServer*)caller;
	CcLogDebugS("Accepted TCP endpoint: " << server->GetRemote());
	this->iOnAccept.Execute();
}
		
void ClTobiIc::HandleDrop(CcSocket* caller) { 
	CcServer *server = (CcServer*)caller;
	CcLogDebugS("Dropped TCP endpoint: " << server->GetRemote());
	this->_hasmessage.Post();
	this->iOnDrop.Execute();
}

#endif
