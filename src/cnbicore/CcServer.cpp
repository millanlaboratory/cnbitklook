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

#ifndef CCSERVER_CPP 
#define CCSERVER_CPP 

#include "CcServer.hpp" 
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

CcServer::CcServer(size_t bsize) : CcSocket(bsize) {
}

CcServer::~CcServer(void) {
}
		
bool CcServer::Bind(CcPort port, int protocol) {
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
	
	int statusb = 0;
	statusb = tr_bind(CcSocket::_socket, port.c_str());
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
	
	if(this->Listen() == false)
		return false;
	CcSocket::_semsocket.Post();
	
	CcThread::Start();
	return true;
}

bool CcServer::Bind(CcPortUInt port, int protocol) {
	//CcEndpoint ep("0.0.0.0", port);
	//CcLogWarningS(ep.GetPort());
	//return this->Bind(ep.GetPort(), protocol);
	return false;
}

bool CcServer::Release(void) {
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
	
	this->iOnRelease.Execute(this);
	return true;
}
		
ssize_t CcServer::Send(const char* message) {
	return this->Send((void*)message, strlen(message));
}

ssize_t CcServer::Send(const std::string& message) {
	return this->Send((void*)message.c_str(), message.size());
}

ssize_t CcServer::Send(const void* message, size_t size) {
	CcSocket::_semsocket.Wait();
	CcSocketMap2It pit;
	ssize_t sendtot = 0;
	for(pit = this->_peers.begin(); pit != this->_peers.end(); pit++) {
		sendtot += CcSocket::Send(pit->second, message, size);
	}
	CcSocket::_semsocket.Post();
	
	return sendtot;
}
		
ssize_t CcServer::Send(const char* message, CcAddress addr) {
	return this->Send((void*)message, strlen(message), addr);
}

ssize_t CcServer::Send(const std::string& message, CcAddress addr) {
	return this->Send((void*)message.c_str(), message.size(), addr);
}

ssize_t CcServer::Send(const void* message, size_t size, CcAddress addr) {
	CcSocket::_semsocket.Wait();
	tr_socket* peer = GetPeer(addr);
	if(peer == NULL) {
		CcLogErrorS("No peer found for: " << addr);
		CcSocket::_semsocket.Post();
		return TR_BYTES_NONE;
	}

	ssize_t bytes = CcSocket::Send(peer, message, size); 
	CcSocket::_semsocket.Post();

	return bytes;
}
		
ssize_t CcServer::SendNot(const char* message, CcAddress addr) {
	return this->SendNot((void*)message, strlen(message), addr);
}

ssize_t CcServer::SendNot(const std::string& message, CcAddress addr) {
	return this->SendNot((void*)message.c_str(), message.size(), addr);
}

ssize_t CcServer::SendNot(const void* message, size_t size, CcAddress addr) {
	CcSocket::_semsocket.Wait();
	CcSocketMap2It pit;
	tr_socket* peer = GetPeer(addr);
	if(peer == NULL) {
		CcLogErrorS("No peer found for: " << addr);
		CcSocket::_semsocket.Post();
		return TR_BYTES_NONE;
	}

	ssize_t sendtot = 0;
	for(pit = this->_peers.begin(); pit != this->_peers.end(); pit++) {
		if(pit->second->fd != peer->fd)
			sendtot += CcSocket::Send(pit->second, message, size);
	}
	CcSocket::_semsocket.Post();
	
	return sendtot;
}

void CcServer::Main(void) {
	if(!CcThread::IsRunning())
		return;
	
	fd_set masterfds, readfds;
	FD_ZERO(&masterfds);
	
	int fdmax = CcSocket::_socket->fd,
		fdlis = CcSocket::_socket->fd,
		fdnew = 0;
	tr_socket* peer = NULL;
	FD_SET(fdlis, &masterfds);

	int status;
	while(CcThread::IsRunning()) {
		readfds = masterfds;
		status = select(fdmax + 1, &readfds, NULL, NULL, NULL); 
		if(status == -1) {
			CcLogFatalS("Async I/O error: " << strerror(status));
			CcThread::Stop();
		}
		
		int fida, fidr, fidd;
		CcAddress addrd;
		for(int i = 0; i <= fdmax; i++) {
			if(FD_ISSET(i, &readfds) <= 0)
				continue;

			fida = 0;
			fidd = 0;
			fidr = 0;

			CcSocket::_semsocket.Wait();
			if(i == fdlis) {
				fdnew = this->Accept();
				if(fdnew > 0) {
					if(fdnew > fdmax)
						fdmax = fdnew;
					FD_SET(fdnew, &masterfds);
					fida = fdnew;
				}
			} else {
				if(CcSocket::HasPeer(i) == false) {
					CcLogFatalS("ASIO peer table corrupted");
					CcSocket::_semsocket.Post();
					CcThread::Stop();
					break;
				}
				
				peer = GetPeer(i);
				if(CcSocket::Recv(peer) == TR_BYTES_ERROR) {
					FD_CLR(i, &masterfds);
					addrd = CcSocket::GetAddress(i);
					this->Drop(i);
					fidd = i;
				} else {
					fidr = i;
				}
			}
			CcSocket::_semsocket.Post();

			if(fida > 0) {
				this->iOnAccept.Execute(this);
				this->iOnAcceptEndpoint.Execute(this, CcSocket::GetAddress(fida));
			}
			if(fidr > 0) {
				CcSocket::iOnRecv.Execute(this);
				this->iOnRecvEndpoint.Execute(this, CcSocket::GetAddress(fidr));
			}
			if(fidd > 0) {
				this->iOnDrop.Execute(this);
				this->iOnDropEndpoint.Execute(this, addrd);
			}
		}
	}
	FD_ZERO(&readfds);
	FD_ZERO(&masterfds);
	this->Release();
}

void CcServer::Drop(int fid) {
	tr_socket* peer = CcSocket::GetPeer(fid);
	tr_close(peer);
	tr_free(peer);
	CcSocket::RemStream(fid);
	CcSocket::RemPeer(fid);
}
		
bool CcServer::Open(int protocol) {
	switch(protocol) {
		case CcSocket::UDP:
			tr_udpserver(CcSocket::_socket);
			break;
		case CcSocket::TCP:
		default:
			tr_tcpserver(CcSocket::_socket);
			break;
	}
	int status = tr_open(CcSocket::_socket);
	
	if(status == 0)	{
		CcLogDebugS("Socket " << CcSocket::_socket->fd << " opened as " <<
				(protocol == CcSocket::UDP ? "UDP" : "TCP"));
	} else {
		CcLogErrorS("Cannot open socket as " << 
				(protocol == CcSocket::UDP ? "UDP" : "TCP"));
	}
	return(status == 0);
}
		
bool CcServer::Listen(void) {
	if(tr_listen(CcSocket::_socket) != 0) {
		CcLogError("Cannot listen on socket");
		return false;
	}
	
	CcLogDebugS("Socket " << CcSocket::_socket->fd << " listening");
	this->iOnListen.Execute(this);

	return true;
}

int CcServer::Accept(void) {
	tr_socket* peer = new tr_socket;
	tr_init_socket(peer, CCCORE_1MB, 1);
	tr_tcpendpoint(peer);
	
	int fid = tr_accept(CcSocket::_socket, peer);
	if(fid <= 0)
		return fid;

	tr_set_nonblocking(peer, 1);
	CcSocket::AddPeer(peer);
	CcSocket::AddStream(fid);
	
	return fid;
}

#endif
