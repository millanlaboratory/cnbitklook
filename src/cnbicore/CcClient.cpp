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

CcClient::CcClient(size_t bsize) : CcSocket(bsize) {
}

CcClient::~CcClient(void) {
	this->Disconnect();
}
		
bool CcClient::Connect(CcAddress address, int protocol) {
	CcSocket::_semsocket.Wait();
	if(CcThread::IsRunning() == true) {
		CcLogWarning("Client thread already running");
		return true;
	}
		
	CcSocket::Close();
	if(this->Open(protocol) == false) {
		CcLogError("Cannot open socket");
		CcSocket::_semsocket.Post();
		return false;
	}
	
	CcEndpoint cache(address);
	
	int status = 0;
	status = tr_connect(CcSocket::_socket, 
			cache.GetIp().c_str(), cache.GetPort().c_str());
	tr_set_nonblocking(CcSocket::_socket, 1);
	CcSocket::AddStream(CcSocket::_socket->fd);
	CcSocket::AddPeer(CcSocket::_socket);
	
	if(status != 0) {
		CcSocket::_semsocket.Post();
		CcLogError("Cannot connect socket");
		return false;
	}
	
	CcEndpoint local, remote;
	local.Set(&CcSocket::_socket->local);
	remote.Set(&CcSocket::_socket->remote);
	CcLogDebugS("Socket " << CcSocket::_socket->fd << " connected: " << 
			local.GetAddress() << " to " << remote.GetAddress());
	CcSocket::_semsocket.Post();

	CcThread::Start();
	
	this->iOnConnect.Execute(this);
	return true;
}

bool CcClient::Disconnect(void) {
	if(this->IsConnected() == false) {
		CcLogWarning("Socket not connected");
		return false;
	}

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
	this->iOnDisconnect.Execute(this);
	return true;
}

ssize_t CcClient::Send(const char* message) {
	CcSocket::_semsocket.Wait();
	ssize_t bytes = CcSocket::Send(CcSocket::_socket, (void*)message,
			strlen(message));
	CcSocket::_semsocket.Post();
	
	return bytes;
}

ssize_t CcClient::Send(const std::string& message) {
	CcSocket::_semsocket.Wait();
	ssize_t bytes = CcSocket::Send(CcSocket::_socket, (void*)message.c_str(),
			message.size());
	CcSocket::_semsocket.Post();
	
	return bytes;
}
		
ssize_t CcClient::Send(const void* message, size_t size) {
	CcSocket::_semsocket.Wait();
	ssize_t bytes = CcSocket::Send(CcSocket::_socket, message, size);
	CcSocket::_semsocket.Post();

	return bytes;
}
		
void CcClient::Main(void) {
	if(!CcThread::IsRunning())
		return;
	
	struct timeval tv;
	fd_set readfds;
	FD_ZERO(&readfds);

	int status;
	bool received;
	while(CcThread::IsRunning()) {
		FD_SET(CcSocket::_socket->fd, &readfds);
		tv.tv_sec = CCCORE_ASIO_SEC;
		tv.tv_usec = CCCORE_ASIO_USEC;
		
		received = false;

		status = select(CcSocket::_socket->fd + 1, &readfds, NULL, NULL, &tv);
		if(status == -1) {
			CcLogFatalS("Async I/O error: " << strerror(status));
			CcThread::Stop();
		} else if(status > 0) {
			if(FD_ISSET(CcSocket::_socket->fd, &readfds)) {
				CcSocket::_semsocket.Wait();
				if(CcSocket::Recv(CcSocket::_socket) == TR_BYTES_ERROR) 
					CcThread::Stop();
				else 
					received = true;
				CcSocket::_semsocket.Post();
			}
		}
		if(received) 
			CcSocket::iOnRecv.Execute(this);
	}
	FD_ZERO(&readfds);
	this->Disconnect();
}

bool CcClient::Open(int protocol) {
	int status = 0;
	switch(protocol) {
		case CcSocket::UDP:
			tr_udpclient(CcSocket::_socket);
			break;
		case CcSocket::TCP:
		default:
			tr_tcpclient(CcSocket::_socket);
			break;
	}
	status = tr_open(CcSocket::_socket);
	
	if(status == 0)	{
		CcLogDebugS("Socket " << CcSocket::_socket->fd << " opened as " <<
				(protocol == CcSocket::UDP ? "UDP" : "TCP"));
	} else {
		CcLogErrorS("Cannot open socket as " << 
				(protocol == CcSocket::UDP ? "UDP" : "TCP"));
	}
	return(status == 0);
}
		
#endif
