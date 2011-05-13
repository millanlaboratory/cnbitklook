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

#ifndef CLCLIENT_CPP 
#define CLCLIENT_CPP 

#include "ClClient.hpp" 

ClClient::ClClient(const double waitms) {
	CB_CcSocket(this->_client.iOnDisconnect, this, HandleDisconnect);
	this->_waitms = waitms;
}

ClClient::~ClClient(void) {
	if(this->_client.IsConnected())
		this->_client.Disconnect();
}
	
bool ClClient::Connect(const CcAddress address) {
	if(this->_client.IsConnected())
		return true;
	try {
		this->_client.Connect(address);
	} catch(CcException e) {
		return false;
	}
	return true;
}

void ClClient::Disconnect(void) {
	this->_client.Disconnect();
}

bool ClClient::IsConnected(void) {
	return this->_client.IsConnected();
}

void ClClient::HandleDisconnect(CcSocket* caller) { 
	CcClient *client = (CcClient*)caller;
	CcAddress address = client->GetLocal();
	CcLogDebugS("Endpoint dropped: " << address);
}

#endif
