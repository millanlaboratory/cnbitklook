/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCSERVERMULTI_CPP
#define CCSERVERMULTI_CPP

#include "CcServerMulti.hpp"
#include "CcBasic.hpp"
#include <libtransport/tcp.h>
#include <string.h>
#include <stdio.h>
#include <list>

CcServerMulti::CcServerMulti(bool multistream, unsigned int bsize, 
		unsigned maxconns) : CcServer(CcServer::AsServer, bsize, maxconns) {
	CcSocket::SetName("CcServerMulti");

	this->OpenSocket();
	this->_endpoints.clear();
	this->_hosts.clear();

	this->_endpointPtr = NULL;
	this->_multistream.Set(multistream);
}

CcServerMulti::~CcServerMulti(void) {
	this->Drop();
	
	if(this->_endpointPtr != NULL) {
		tr_free(this->_endpointPtr);
		delete this->_endpointPtr;
	}
}

void CcServerMulti::Main(void) {
	if(!CcThread::IsRunning())
		return;

	CcAddress address;
	CcTimeValue acceptdt;
	CcTime::Tic(&acceptdt);
	while(CcThread::IsRunning()) {
		CcSocket::_semsocket.Wait();
		tr_set_nonblocking(this->_socket, 1);
		CcSocket::_semsocket.Post();
		
		while(CcThread::IsRunning()) {
			if(this->Recv() >= 0) 
				continue;
			CcTime::Sleep(CCASYNC_WAIT_MRECV);
			
			if(CcTime::Toc(&acceptdt) < CCASYNC_WAIT_MACCEPT)
				continue;

			if(this->Accept(&address)) {
				this->_semendpoints.Wait();
				if(this->ImplHasEndpoint(address)) {
					CcSocketMapIt it = this->_endpoints.find(address);
					tr_set_nonblocking(&(*it->second), 1);
				}
				this->_semendpoints.Post();
			}
			CcTime::Tic(&acceptdt);
		}
	}
	this->Drop();
}

bool CcServerMulti::Accept(CcAddress* raddress) {
	CcSocket::_semsocket.Wait();
	this->_semendpoints.Wait();
	
	/* To accept I need a fresh endpoint. Let _endpointPtr 
	 * keep a fresh one for the next accept
	 */
	std::string address;
	tr_socket* endpoint = NULL;
	if(this->_endpointPtr == NULL) {
		this->_endpointPtr = new tr_socket;
		tr_init_socket(this->_endpointPtr, CcSocket::_socket->bsize * sizeof(char), 1);
		tr_tcpendpoint(this->_endpointPtr);
	}
	endpoint = this->_endpointPtr;

	bool status = (tr_accept(CcSocket::_socket, endpoint) > 0);
	if(status) {
		CcEndpoint* host = new CcEndpoint();
		std::string* addressH = new std::string;
		std::string* addressE = new std::string;
		std::string* addressS = new std::string;

		host->Set(&endpoint->remote);
		address.assign(host->GetAddress());
		addressH->assign(host->GetAddress());
		addressE->assign(host->GetAddress());
		addressS->assign(host->GetAddress());
		
		if(this->ImplHasEndpoint(address) == true) {
			std::string info;
			info += "Endpoint ";
			info += address;
			info += " already present in endpoint map";
			CcThrow(info);
		} else
			this->_endpoints[*addressE] = endpoint;

		if(this->ImplHasHost(address) == true) {
			std::string info;
			info += "Host ";
			info += address;
			info += " already present in host map";
			std::cout << info << std::endl;
			CcThrow(info);
		} else
			this->_hosts[*addressH] = host;
		
		if(this->_multistream.Get() == true) {
			this->_semdatastreams.Wait();
			this->datastreams[*addressS] = new CcStreamer();
			this->_semdatastreams.Post();
		}
		this->_endpointPtr = NULL;
	}
	CcSocket::_semsocket.Post();
	this->_semendpoints.Post();
	
	if(status) {
		this->iOnAccept.Execute((CcSocket*)this);
		this->iOnAcceptEndpoint.Execute((CcSocket*)this, (CcAddress)address);
		this->pOnAccept();
	}
	if(raddress != NULL) {
		if(status) 
			raddress->assign(address);
		else
			raddress->clear();
	}

	return status;
}

int CcServerMulti::Recv(void) {
	int bytes = TR_BYTES_NONE;
	
	this->_semendpoints.Wait();
	/* During Recv we have two handle two things:
	 * 1. An endpoint sent a message
	 * 2. An endpoint dropped the connection
	 * We then use two lists of addresses in which we write for each
	 * endpoint its CcAddress.
	 * After looping through all endpoints, we will drop what needed and we will
	 * trigger all the callbacks
	 */
	std::list<CcAddress>::iterator ite;
	std::list<CcAddress> erecv;
	std::list<CcAddress> edrop;

	CcSocketMapIt it;
	CcSocketMapIt begin;
	CcSocketMapIt end;
	begin = this->_endpoints.begin();
	end = this->_endpoints.end();

	// Receive from all endpoints, and check who dropped
	for(it = begin; it != end; it++) {
		bytes = this->ImplRecv(std::string(it->first));
		if(bytes > 0) {
			erecv.push_back(std::string(it->first));
		} else if(bytes == TR_BYTES_ERROR) {
			edrop.push_back(std::string(it->first));
		}
	}

	// Drop endpoints
	for(ite = edrop.begin(); ite != edrop.end(); ite++)
		this->ImplDrop((CcAddress)*ite);
	this->_semendpoints.Post();
	
	// Enter generic drop callback
	if(edrop.begin() != edrop.end()) {
		this->pOnDrop();
		this->iOnDrop.Execute((CcSocket*)this);
		// Enter per-endpoint drop callback
		for(ite = edrop.begin(); ite != edrop.end(); ite++) 
			this->iOnDropEndpoint.Execute((CcSocket*)this, (CcAddress)*ite);
	}

	// Enter generic recv callback
	if(erecv.begin() != erecv.end()) {
		this->pOnRecv();
		this->iOnRecv.Execute((CcSocket*)this);
		// Enter per-endpoint recv callback
		for(ite = erecv.begin(); ite != erecv.end(); ite++)
			this->iOnRecvEndpoint.Execute((CcSocket*)this, (CcAddress)*ite);
	}
	
	return bytes;
}

void CcServerMulti::Drop(void) {
	this->_semendpoints.Wait();
	CcSocketMapIt it;
	CcSocketMapIt begin = this->_endpoints.begin();
	CcSocketMapIt end = this->_endpoints.end();
	
	std::list<CcAddress> edrop;
	std::list<CcAddress>::iterator ite;

	for(it = begin; it != end; it++) {
		if(this->ImplDrop(std::string(it->first)) >= 0)
			edrop.push_back(std::string(it->first));
	}
	this->_semendpoints.Post();

	if(edrop.begin() != edrop.end()) {
		this->pOnDrop();
		this->iOnDrop.Execute((CcSocket*)this);
	}
	for(ite = edrop.begin(); ite != edrop.end(); ite++)
		this->iOnDropEndpoint.Execute((CcSocket*)this, (CcAddress)*ite);
}

int CcServerMulti::ImplDrop(CcAddress address) {
	this->_semhosts.Wait();
	this->_semdatastreams.Wait();

	CcSocketMapIt   itE = this->_endpoints.find(address);
	CcEndpointMapIt itH = this->_hosts.find(address);
	CcStreamerMapIt itS = this->datastreams.find(address);
	
	int status = tr_check(&(*itE->second));
	if(status == 1) 
		tr_close(&(*itE->second));
	tr_free(&(*itE->second));
	
	this->_endpoints.erase(itE);
	this->_hosts.erase(itH);
	this->datastreams.erase(itS);
	this->_semhosts.Post();
	this->_semdatastreams.Post();

	return status;
}

int CcServerMulti::Send(const char* message) {
	this->_semendpoints.Wait();
	int bytes = TR_BYTES_NONE;

	CcSocketMapIt it;
	CcSocketMapIt begin = this->_endpoints.begin();
	CcSocketMapIt end = this->_endpoints.end();

	for(it = begin; it != end; it++)
		bytes += this->ImplSend(message, std::string(it->first));
	this->_semendpoints.Post();

	return bytes;
}

int CcServerMulti::Send(const char* message, CcAddress address) {
	this->_semendpoints.Wait();
	int bytes = this->ImplSend(message, address);
	this->_semendpoints.Post();
	
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE)
		return bytes;
	
	this->iOnSendEndpoint.Execute((CcSocket*)this, (CcAddress)address);
	
	return bytes;
}

int CcServerMulti::SendNot(const char* message, CcAddress address) {
	this->_semendpoints.Wait();
	int bytes = TR_BYTES_NONE;

	CcSocketMapIt it;
	CcSocketMapIt begin = this->_endpoints.begin();
	CcSocketMapIt end = this->_endpoints.end();
	CcAddress cache;

	for(it = begin; it != end; it++) {
		if((it->first).compare(address) == 0)
			continue;
		bytes += this->ImplSend(message, std::string(it->first));
	}

	this->_semendpoints.Post();

	return bytes;
}

int CcServerMulti::SendNot(std::string* message, CcAddress address) {
	this->_semendpoints.Wait();
	int bytes = TR_BYTES_NONE;

	CcSocketMapIt it;
	CcSocketMapIt begin = this->_endpoints.begin();
	CcSocketMapIt end = this->_endpoints.end();
	CcAddress cache;

	for(it = begin; it != end; it++) {
		if((it->first).compare(address) == 0)
			continue;
		bytes += this->ImplSend((const char*)message->c_str(), std::string(it->first));
	}

	this->_semendpoints.Post();

	return bytes;
}

int CcServerMulti::ImplSend(const char* message, CcAddress address) {
	if(strlen(message) > CcSocket::_socket->bsize)
		CcLogWarning("Message size larger than socket buffer");
	
	CcSocketMapIt it = this->_endpoints.find(address);
	int bytes = tr_send(&(*it->second), (char*)message);
	
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE)
		return bytes;

	this->AddBytesSend(bytes);
	this->pOnSend();
	this->iOnSend.Execute((CcSocket*)this);
	
	return bytes;
}

int CcServerMulti::Send(std::string* message) {
	return this->Send((const char*)message->c_str());
}

int CcServerMulti::Send(std::string* message, CcAddress address) {
	return this->Send((const char*)message->c_str(), address);
}

//TODO lock datastream
bool CcServerMulti::SendRecv(std::string *query, std::string *reply,
	std::string hdr, std::string trl, float waitms) {
	if(this->_multistream.Get() == false) {
		std::string message;
		message.append("Multistream enabled, method not usable");
		CcThrow(message);
	}

	int bytes = this->Send(query);
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE)
		return false;
	
	CcTimeValue dtms;
	CcTime::Tic(&dtms);
	while(this->datastream.Has(hdr, trl, CcStreamer::Reverse) == false) {
		if(CcTime::Toc(&dtms) > waitms && waitms >= 0)
			return false;
		else if(waitms == 0)
			return false;
		CcTime::Sleep(CCASYNC_WAIT_RECV);
	}
	this->datastream.Extract(reply, hdr, trl, CcStreamer::Reverse);

	return true;
}

		
int CcServerMulti::ImplRecv(CcAddress address) {
	int bytes = TR_BYTES_NONE;

	CcSocket::_sembuffer.Wait();
	CcSocket::_semsocket.Wait();
	CcSocketMapIt it = this->_endpoints.find(address);
	bytes = tr_recv(&(*it->second));
	memcpy(CcSocket::_buffer, (*it->second).buffer,
			CcSocket::_socket->bsize * sizeof(char));
	CcSocket::_semsocket.Post();

	if(bytes == TR_BYTES_NONE) {
		CcSocket::_sembuffer.Post();
		return bytes;
	}
	if(bytes == TR_BYTES_ERROR) {
		CcSocket::_sembuffer.Post();
		return bytes;
	}
	
	if(this->_multistream.Get() == true) {
		this->_semdatastreams.Wait();
		CcStreamerMapIt itS = this->datastreams.find(address);
		(*itS->second).Append(CcSocket::_buffer);
		this->_semdatastreams.Post();
	} else
		CcSocket::datastream.Append(CcSocket::_buffer);
	CcSocket::_sembuffer.Post();

	this->AddBytesRecv(bytes);
	return bytes;
}


bool CcServerMulti::ImplHasHost(CcAddress address) {
	if(this->_hosts.empty())
		return false;

	CcEndpointMapCIt it = this->_hosts.find(address);
	return(it != this->_hosts.end());
}

bool CcServerMulti::ImplHasEndpoint(CcAddress address) {
	if(this->_endpoints.empty())
		return false;
	
	CcSocketMapCIt it = this->_endpoints.find(address);
	return(it != this->_endpoints.end());
}

void CcServerMulti::DumpEndpoints(void) {
	this->_semendpoints.Wait();
	CcSocketMapIt it;
	CcSocketMapIt begin = this->_endpoints.begin();
	CcSocketMapIt end = this->_endpoints.end();

	for(it = begin; it != end; it++)
		std::cout << " " << it->first << std::endl;
	this->_semendpoints.Post();
}

CcEndpointList CcServerMulti::GetRemote(void) {
	CcEndpointList vector;

	this->_semendpoints.Wait();
	CcSocketMapIt it;
	CcSocketMapIt begin = this->_endpoints.begin();
	CcSocketMapIt end = this->_endpoints.end();

	for(it = begin; it != end; it++)
		vector.push_back(std::string(it->first));
	this->_semendpoints.Post();

	return vector;
}

#endif
