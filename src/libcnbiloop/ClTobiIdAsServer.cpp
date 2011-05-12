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

#ifndef CLTOBIIDASSERVER_CPP 
#define CLTOBIIDASSERVER_CPP 

#include "ClTobiIdAsServer.hpp" 
#include "ClAcqLang.hpp"
#include <iostream>
#include <fstream>

ClTobiIdAsServer::ClTobiIdAsServer(CaWriter* writer, ndf_frame* frame,
		CcSemaphore* semframe) {
	this->_writer = writer;
	this->_frame = frame;
	this->_semframe = semframe;
	this->_serializerD = new IDSerializerRapid(&this->_messageD);
}

ClTobiIdAsServer::~ClTobiIdAsServer(void) {
	if(this->_serializerD == NULL)
		return;
	delete this->_serializerD;
	this->_serializerD = NULL;
}

void ClTobiIdAsServer::HandleBind(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS("Bound TCP socket: " << server->GetLocal());
}

void ClTobiIdAsServer::HandleRelease(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS("Released TCP socket: " << server->GetLocal());
}

void ClTobiIdAsServer::HandleListen(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS(
			"Listening on TCP socket: " << server->GetLocal());
}

void ClTobiIdAsServer::HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS("Accepted TCP endpoint: " << address);
}

void ClTobiIdAsServer::HandleDropEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS("Dropped TCP endpoint: " << address);
}

void ClTobiIdAsServer::HandleRecvEndpoint(CcSocket* caller, CcAddress address) { 
	CcServerMulti* server = (CcServerMulti*)caller;

	while(this->CommunicationTiD(server, address));
}

bool ClTobiIdAsServer::CommunicationTiD(CcServerMulti* server, CcAddress address) {
	int idblock = TCBlock::BlockIdxUnset, ndfblock = TCBlock::BlockIdxUnset;
	IDevent idevent;
	IDFtype idfamily;
	std::string message;

	if(server->datastreams[address]->Extract(&message, 
			"<tobiid", "/>", CcStreamer::Forward) == false)
		return false;

	this->_serializerD->Deserialize(&message);
	idevent = this->_messageD.GetEvent();
	idfamily = this->_messageD.GetFamilyType();
	idblock = this->_messageD.GetBlockIdx();
	
	// Verify family type for compatibility
	if(idfamily != IDMessage::FamilyBiosig) {
		CcLogWarning("TiD family type not supported");
		return true;
	}
	
	if(this->_messageD.absolute.IsSet()) {
		this->_writer->Tic(&this->_messageD);
		this->_semframe->Wait();
		if(idblock == TCBlock::BlockIdxUnset) {
			ndfblock = ndf_get_fidx(this->_frame);
			this->_messageD.SetBlockIdx(ndfblock);
		}
		if(ndf_add_label(this->_frame, &idevent) == NULL)
			CcLogError("TiD event is valid but cannot be added to NDF frame");
		this->_writer->AddEvent(idevent);
		this->_semframe->Post();

		/* Perform all the necessary operations on the timestamps
		 * and distribute to all connected clients but the sender
		 */
		this->_serializerD->Serialize(&message);
		server->SendNot(message.c_str(), address);
		
		CcLogInfoS("TiD event from " << address << " distributed: " <<
					this->_messageD.GetDescription() << "::" <<
					idevent << "@" <<
					idblock << "/" << ndfblock << 
					" (" << this->_writer->TocOpen()/1000 << "s)");
	} else {
		CcLogError("TiD event is not valid and will not be distributed");
	}
	return true;
}

void ClTobiIdAsServer::Register(CcServerMulti* server) {
	CB_CcSocket(server->iOnBind, this, HandleBind);
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnListen, this, HandleListen);
	CB_CcSocket(server->iOnAcceptEndpoint, this, HandleAcceptEndpoint);
	CB_CcSocket(server->iOnDropEndpoint, this, HandleDropEndpoint);
	CB_CcSocket(server->iOnRecvEndpoint, this, HandleRecvEndpoint);
}
#endif
