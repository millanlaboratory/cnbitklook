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

#ifndef CCUDPLISTENER_CPP 
#define CCUDPLISTENER_CPP 

#include "CcUDPListener.hpp" 
#include "CcEndpoint.hpp"
#include "CcTime.hpp"
#include "CcLog.hpp"
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <transport/tr_net.h>
#include <transport/tr_tcp.h>
#include <transport/tr_udp.h>

CcUDPListener::CcUDPListener(size_t bsize) : CcSocket(bsize) {
}

CcUDPListener::~CcUDPListener(void) {
}
		
bool CcUDPListener::Bind(CcIp ip, CcPort port) {
	CcSocket::_semsocket.Wait();
	if(CcThread::IsRunning() == true) {
		CcLogWarning("Client thread already running");
		return true;
	}
	
	CcSocket::Close();
	if(this->Open(CcSocket::UDP) == false) {
		CcLogError("Cannot open socket");
		CcSocket::_semsocket.Post();
		return false;
	}
	
	int statusb = 0;
	statusb = tr_bind(CcSocket::_socket, ip.c_str(), port.c_str());
	tr_set_nonblocking(CcSocket::_socket, 1);
	CcSocket::AddStream(CcSocket::_socket->fd);
	CcSocket::AddPeer(CcSocket::_socket);
	
	if(statusb != 0) {
		CcSocket::_semsocket.Post();
		CcLogError("Cannot bind socket");
		return false;
	}
	
	CcEndpoint local;
	local.Set(&CcSocket::_socket->local);
	CcLogDebugS("Socket " << CcSocket::_socket->fd << " bound: " <<
			local.GetAddress());
	this->iOnBind.Execute(this);
	CcSocket::_semsocket.Post();
	CcThread::Start();
	return true;
}
		
bool CcUDPListener::Bind(CcAddress address) {
	CcEndpoint ep(address);
	return this->Bind(ep.GetIp(), ep.GetPort());
}

bool CcUDPListener::Release(void) {

	if(CcThread::IsRunning()) {
		CcThread::Stop();
		CcThread::Join();
	}

	CcSocket::_semsocket.Wait();
	if(this->Close() == false) {
		CcSocket::_semsocket.Post();
		return false;
	}
	CcSocket::RemStream(CcSocket::_socket->fd);
	CcSocket::_semsocket.Post();
	
	this->iOnRelease.Execute(this);
	return true;
}

void CcUDPListener::Main(void) {
	if(!CcThread::IsRunning())
		return;
	
	while(CcThread::IsRunning()) {

		CcAddress address;
		CcStreamer* stream = NULL;
		address = CcSocket::GetAddress(this->_socket->fd);
		stream = CcSocket::GetStream(this->_socket->fd);
		
		CcSocket::_semsocket.Wait();
		ssize_t readsize = CcSocket::Recv(this->_socket);
		CcSocket::_semsocket.Post();
		//std::cout<<"ReadSize = "<<readsize<<std::endl;
		if(readsize > 0){		
			CcSocket::iOnRecv.Execute(this, stream);
			this->iOnRecvPeer.Execute(this, address, stream);
		}
	}
	this->Release();
}
		
bool CcUDPListener::Open(int protocol) {
	
	tr_udpserver(CcSocket::_socket);
	int status = tr_open(CcSocket::_socket);
	if(status == 0)	{
		CcLogDebugS("Socket " << CcSocket::_socket->fd << " opened as UDP");
	} else {
		CcLogErrorS("Cannot open socket as UDP");
	}
	return(status == 0);
}

ssize_t CcUDPListener::Send(const char* message) {
	return this->Send((void*)message, strlen(message));
}

ssize_t CcUDPListener::Send(const std::string& message) {
	return this->Send((void*)message.c_str(), message.size());
}

ssize_t CcUDPListener::Send(const void* message, size_t size) {
	CcSocket::_semsocket.Wait();
	CcSocketMapIt pit;
	ssize_t sendtot = 0;
	for(pit = this->_peers.begin(); pit != this->_peers.end(); pit++) {
		sendtot += CcSocket::Send(pit->second, message, size);
	}
	CcSocket::_semsocket.Post();
	
	return sendtot;
}

void* CcUDPListener::GetBuffer(void) {
	CcSocket::_semsocket.Wait();
	void* sbuf = this->_socket->buffer;
	CcSocket::_semsocket.Post();
	return sbuf;
}

#endif
