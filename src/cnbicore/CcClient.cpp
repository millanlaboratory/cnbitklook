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

#ifndef CCCLIENT_CPP
#define CCCLIENT_CPP
		
#include "CcClient.hpp"
#include <transport/tr_tcp.h>
#include <stdio.h>
#include <string.h>

CcClient::CcClient(size_t bsize, unsigned int maxconns) 
	: CcSocket(bsize, maxconns) {
}

CcClient::~CcClient(void) {
	this->Disconnect();
}
		
void CcClient::OpenSocket(void) {
	int status;
	CcSocket::_semsocket.Wait();
	tr_tcpclient(CcSocket::_socket);
	status = tr_open(CcSocket::_socket);
	CcSocket::_semsocket.Post();
	
	if(status == -1)
		CcThrow("Can not open socket");
}

void CcClient::CloseSocket(void) {
	int status;
	CcSocket::_semsocket.Wait();
	status = tr_close(CcSocket::_socket);
	CcSocket::_semsocket.Post();
	
	if(status > 0)
		CcThrow("Can not close socket");
}

void CcClient::Connect(const CcEndpoint endpoint, const unsigned int wait) { 
	this->Connect(endpoint.GetIp(), endpoint.GetPortUInt(), wait);
}

void CcClient::Connect(const CcAddress address, const unsigned int wait) { 
	CcEndpoint cache(address);
	this->Connect(cache.GetIp(), cache.GetPortUInt(), wait);
}

void CcClient::Connect(const CcIp ip, const CcPortUInt port, 
		const unsigned int wait) {
	if(CcThread::IsRunning() == true)
		return;

	this->OpenSocket();
	
	unsigned int waited = 0;
	char sport[5];
	sprintf(sport, "%u", port);

	int status;
	while(waited <= wait) {
		CcSocket::_semsocket.Wait();
		status = tr_connect(CcSocket::_socket, ip.c_str(), sport);
		tr_set_nonblocking(CcSocket::_socket, 1);
		this->_hostLocal.Set(&(CcSocket::_socket->local));
		this->_hostRemote.Set(&(CcSocket::_socket->remote));
		CcSocket::_semsocket.Post();
		
		if(status == 0) 
			break;
		CcTime::Sleep(CCASYNC_WAIT_CONNECT);
		++waited;
	}

	if(status < 0)
		CcThrow("Can not connect socket");

	this->iOnConnect.Execute((CcSocket*)this);
	this->pOnConnect();
	CcThread::Start();
	while(CcThread::IsRunning() == false) 
		CcTime::Sleep(CCASYNC_WAIT_CONNECT);
}

void CcClient::Disconnect(void) {
	if(this->IsConnected() == false)
		return;

	if(CcThread::IsRunning()) {
		CcThread::Stop();
		CcThread::Join();
	}

	this->CloseSocket();
	this->iOnDisconnect.Execute((CcSocket*)this);
	this->pOnDisconnect();
}

int CcClient::Send(const char* message) {
	if(CcThread::IsRunning() == false)
		return -1;
	
	int bytes = TR_BYTES_NONE;

	CcSocket::_semsocket.Wait();
	bytes = tr_send(CcSocket::_socket, (char*)message);
	CcSocket::_semsocket.Post();
	
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE)
		return bytes;
	
	this->AddBytesSend(bytes);
	this->pOnSend();
	CcSocket::iOnSend.Execute((CcSocket*)this);
	
	return bytes;
}

int CcClient::Send(std::string* message) {
	return this->Send((const char*)message->c_str());
}

bool CcClient::SendRecv(const char* query, std::string *reply,
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
		
bool CcClient::SendRecv(const std::string& query, std::string *reply, 
				std::string hdr, std::string trl, float waitms) {
	return this->SendRecv(query.c_str(), reply, hdr, trl, waitms);
}

int CcClient::Recv(void) {
	if(CcThread::IsRunning() == false)
		return -1;

	int bytes = TR_BYTES_NONE;
	int status = 0;
	
	CcSocket::_semsocket.Wait();
	status = tr_check(CcSocket::_socket);
	if(status <= 0) {
		CcLogFatal("Socket is broken");
		CcSocket::_semsocket.Post();
		return status;
	}

	CcSocket::_sembuffer.Wait();
	bytes = tr_recvb(CcSocket::_socket, CcSocket::_buffer, CcSocket::_bsize); 
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE) {
		CcSocket::_semsocket.Post();
		CcSocket::_sembuffer.Post();
		return bytes;
	}
	CcSocket::_semsocket.Post();
	
	CcSocket::datastream.Append(CcSocket::_buffer, bytes);
	CcSocket::_sembuffer.Post();

	this->AddBytesRecv(bytes);
	this->pOnRecv();
	CcSocket::iOnRecv.Execute((CcSocket*)this);
		
	return bytes;
}


void CcClient::Main(void) {
	if(!CcThread::IsRunning())
		return;

	int bytes;
	while(CcThread::IsRunning()) {
		bytes = this->Recv();
		switch(bytes) {
			case TR_BYTES_NONE:
				CcTime::Sleep(CCASYNC_WAIT_RECV);
				break;
			case TR_BYTES_ERROR:
				CcThread::Stop();
				break;
			default:
				continue;
		}
	}
	this->Disconnect();
}

CcAddress CcClient::GetLocal(void) {
	CcSocket::_semsocket.Wait();
	CcAddress id = this->_hostLocal.GetAddress();
	CcSocket::_semsocket.Post();
	return id;
}

CcAddress CcClient::GetRemote(void) {
	CcSocket::_semsocket.Wait();
	CcAddress id = this->_hostRemote.GetAddress();
	CcSocket::_semsocket.Post();
	return id;
}
#endif
