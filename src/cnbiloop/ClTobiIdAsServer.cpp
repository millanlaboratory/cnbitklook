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

void ClTobiIdAsServer::HandleRecvPeer(CcSocket* caller, CcAddress addr, 
		CcStreamer* stream) { 
	while(this->CommunicationTiD((CcServer*)caller, addr, stream));
}

bool ClTobiIdAsServer::CommunicationTiD(CcServer* server, CcAddress address, 
		CcStreamer* stream) {
	int idblock = TCBlock::BlockIdxUnset, ndfblock = TCBlock::BlockIdxUnset;
	IDevent idevent;
	IDFtype idfamily;
	std::string message;

	if(stream->Extract(&message, "<tobiid", "/>") == false)
		return false;

	this->_serializerD->Deserialize(&message);
	idevent = this->_messageD.GetEvent();
	idfamily = this->_messageD.GetFamilyType();
	idblock = this->_messageD.GetBlockIdx();
	
	// Verify family type for compatibility
	if(idfamily != IDMessage::FamilyBiosig) {
		CcLogWarning("Bus/iD family type not supported");
		return true;
	}
	
	if(this->_messageD.absolute.IsSet()) {
		this->_writer->Tic(&this->_messageD);
		this->_semframe->Wait();
		ndfblock = ndf_get_fidx(this->_frame);
		if(idblock == TCBlock::BlockIdxUnset)
			this->_messageD.SetBlockIdx(ndfblock);
		if(ndf_add_label(this->_frame, &idevent) == NULL)
			CcLogError("Bus/iD event is valid but cannot be added to NDF frame");
		this->_writer->AddEvent(idevent);
		this->_semframe->Post();

		/* Perform all the necessary operations on the timestamps
		 * and distribute to all connected clients but the sender
		 */
		this->_serializerD->Serialize(&message);
		server->SendNot(message.c_str(), address);
		
		double onset = this->_writer->TocOpen();
		if(onset <= 0)
			onset = 0;
		CcLogInfoS("Bus/iD: " << this->_messageD.GetDescription() <<
				", Event=" << idevent << 
				", NDF=" << ndfblock << 
				", TiD=" << idblock << 
				", Onset=" << onset/1000 << "s");
	} else {
		CcLogError("Bus/iD event is not valid and will not be distributed");
	}
	return true;
}

void ClTobiIdAsServer::Register(CcServer* server) {
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnAcceptPeer, this, HandleAcceptPeer);
	CB_CcSocket(server->iOnDropPeer, this, HandleDropPeer);
	CB_CcSocket(server->iOnRecvPeer, this, HandleRecvPeer);
}
#endif
