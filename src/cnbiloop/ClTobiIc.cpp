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
}

ClTobiIc::~ClTobiIc(void) {
	ClLoop::Release();
}
		
bool ClTobiIc::Attach(const CcPort port, const std::string& name) {
	if(this->_server != NULL)
		return false;
	
	this->_name.assign(name);

	if(ClLoop::Connect() == false) { 
		CcLogDebug("Cannot connect to loop");
		return false;
	}
	CcEndpoint endpoint("0.0.0.0", port);
	
	this->_server = new CcServerSingle();
	try { 
		this->_server->Bind(endpoint);
	} catch(CcException e) {
		CcLogDebugS("Cannot bind to port " << port);
		return false;
	}

	int status = ClLoop::nameserver.Set(this->_name, endpoint.GetAddress());
	if(status != ClNamesLang::Successful) {
		CcLogDebugS("Cannot set " << name << " as " << endpoint.GetAddress());
		return false;
	}

	CB_CcSocket(this->_server->iOnAccept, this, HandleAccept);
	CB_CcSocket(this->_server->iOnDrop, this, HandleDrop);
	CB_CcSocket(this->_server->iOnRecv, this, HandleRecv);
	
	this->_hasmessage.Wait();

	return true;
}
		
bool ClTobiIc::Detach(void) {
	if(this->_server == NULL)
		return true;

	ClLoop::nameserver.Unset(this->_name);
	this->_server->Release();
	delete(this->_server);
	this->_server = NULL;
	this->_hasmessage.Post();

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

void ClTobiIc::HandleRecv(CcSocket* caller) { 
	if(this->_sembuffer.TryWait() == false) 
		return;

	bool status = false;
	status = this->_server->datastream.Extract(&this->_buffer, "<tobiic",
			"</tobiic>");
	if(status)
		this->_hasmessage.Post();
	this->_sembuffer.Post();
}

void ClTobiIc::HandleAccept(CcSocket* caller) { 
	CcServerSingle *server = (CcServerSingle*)caller;
	CcLogDebugS("Accepted TCP endpoint: " << server->GetRemote());
}
		
void ClTobiIc::HandleDrop(CcSocket* caller) { 
	CcServerSingle *server = (CcServerSingle*)caller;
	CcLogDebugS("Dropped TCP endpoint: " << server->GetRemote());
	this->_hasmessage.Post();
}

#endif
