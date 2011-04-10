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

#ifndef CCSERVERSINGLE_CPP
#define CCSERVERSINGLE_CPP

#include "CcServerSingle.hpp"
#include "CcBasic.hpp"
#include <libtransport/tcp.h>
#include <string.h>
#include <stdio.h>

CcServerSingle::CcServerSingle(size_t bsize, unsigned maxconns) :
	CcServer(CcServer::AsServer, bsize, maxconns) {
	CcSocket::SetName("CcServerSingle");

	this->OpenSocket();
}

CcServerSingle::~CcServerSingle(void) {
	this->Drop();
}

bool CcServerSingle::Accept(void) {
	CcSocket::_semsocket.Wait();
	this->_semendpoint.Wait();
	bool status = (tr_accept(CcSocket::_socket, &this->_endpoint) > 0);
	if(status)
		this->_hostRemote.Set(&(this->_endpoint.remote));
	this->_semendpoint.Post();
	CcSocket::_semsocket.Post();

	if(status) {
		this->iOnAccept.Execute((CcSocket*)this);
		this->pOnAccept();
	}
	return status;
}

void CcServerSingle::Drop(void) {
	this->_semendpoint.Wait();
	int status = tr_check(&this->_endpoint);
	if(status == 1) 
		tr_close(&this->_endpoint);
	tr_free(&this->_endpoint);
	this->_semendpoint.Post();

	if(status < 1)
		return;

	this->iOnDrop.Execute((CcSocket*)this);
	if(status)
		this->pOnDrop();
}

int CcServerSingle::Send(const char* message) {
	int bytes = TR_BYTES_NONE;
	
	if(strlen(message) > CcSocket::_socket->bsize)
		CcLogWarning("Message size larger than socket buffer");

	this->_semendpoint.Wait();
	bytes = tr_send(&this->_endpoint, (char*)message);
	this->_semendpoint.Post();
	
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE)
		return bytes;
	
	this->AddBytesSend(bytes);
	this->pOnSend();
	this->iOnSend.Execute((CcSocket*)this);
	
	return bytes;
}

int CcServerSingle::Send(std::string* message) {
	return this->Send((const char*)message->c_str());
}

bool CcServerSingle::SendRecv(std::string *query, std::string *reply,
	std::string hdr, std::string trl, float waitms) {
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

int CcServerSingle::Recv(void) {
	int bytes = TR_BYTES_NONE;

	CcSocket::_sembuffer.Wait();
	this->_semendpoint.Wait();
	bytes = tr_recv(&this->_endpoint);
	memcpy(CcSocket::_buffer, this->_endpoint.buffer,
			CcSocket::_socket->bsize * sizeof(char));
	this->_semendpoint.Post();

	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE) {
		CcSocket::_sembuffer.Post();
		return bytes;
	}
	
	CcSocket::datastream.Append(CcSocket::_buffer);
	CcSocket::_sembuffer.Post();

	this->AddBytesRecv(bytes);
	this->pOnRecv();
	this->iOnRecv.Execute((CcSocket*)this);
	
	return bytes;
}

void CcServerSingle::Main(void) {
	if(!CcThread::IsRunning())
		return;

	while(CcThread::IsRunning()) {
		CcSocket::_semsocket.Wait();
		tr_set_nonblocking(this->_socket, 1);
		CcSocket::_semsocket.Post();

		this->_semendpoint.Wait();
		tr_init_socket(&this->_endpoint, 
				CcSocket::_socket->bsize * sizeof(char), 1);
		tr_tcpendpoint(&this->_endpoint);
		this->_semendpoint.Post();

		while(CcThread::IsRunning()) {
			if(this->Accept()) {
				this->_semendpoint.Wait();
				tr_set_nonblocking(&this->_endpoint, 1);
				this->_semendpoint.Post();
				break;
			}
			else {
				CcTime::Sleep(CCASYNC_WAIT_ACCEPT);
			}
		}
		
		int bytes;
		while(CcThread::IsRunning()) {
			bytes = this->Recv();
			
			if(bytes > 0)
				continue;
			if(bytes == TR_BYTES_ERROR)
				break;
			CcTime::Sleep(CCASYNC_WAIT_RECV);
		}
		this->Drop();
	}
	this->Drop();
}

CcAddress CcServerSingle::GetRemote(void) {
	this->_semendpoint.Wait();
	CcAddress id = this->_hostRemote.GetAddress();
	this->_semendpoint.Post();

	return id;
}

#endif
