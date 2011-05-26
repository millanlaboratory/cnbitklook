/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    his program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCSOCKET_CPP
#define CCSOCKET_CPP

#include "CcSocket.hpp"
#include "CcBasic.hpp"
#include <stdio.h>
#include <string.h>
#include <transport/tr_names.h>
#include <transport/tr_types.h>

CcSocket::CcSocket(size_t bsize) {
	this->_semsocket.Wait();
	this->_socket = new tr_socket;
	tr_init_socket(this->_socket, bsize, 32);
	this->_semsocket.Post();
}

CcSocket::~CcSocket(void) {
	this->_semsocket.Wait();
	if(tr_check(this->_socket))
		tr_close(this->_socket);
	tr_free(this->_socket);
	delete this->_socket;
	this->_semsocket.Post();
}
		
void CcSocket::AddSend(const unsigned int bytes) {
	this->_bytesSend += bytes;
}

void CcSocket::AddRecv(const unsigned int bytes) {
	this->_bytesRecv += bytes;
}

long unsigned int CcSocket::GetSend(void) {
	long unsigned int bytes = this->_bytesSend;
	return bytes;
}

long unsigned int CcSocket::GetRecv(void) {
	long unsigned int bytes = this->_bytesRecv;
	return bytes;
}

bool CcSocket::IsConnected(void) {
	this->_semsocket.Wait();
	bool status = (tr_connected(this->_socket) == 0);
	this->_semsocket.Post();
	return status;
}

ssize_t CcSocket::Recv(tr_socket* peer) {
	int status = 0;
	status = tr_check(peer);
	if(status <= 0) {
		CcLogFatal("Socket is broken");
		return status;
	}

	ssize_t bytes = tr_recv(peer); 
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE) {
		return bytes;
	}
	this->AddRecv(bytes);
	
	CcStreamer* stream = CcSocket::GetStream(peer->fd);
	if(stream)
		stream->Append((char*)peer->buffer, bytes);
	else
		CcLogWarningS("Stream not found for socket " << peer->fd);
		
	return bytes;
}

ssize_t CcSocket::Send(tr_socket* peer, const void* message, size_t size) {
	int bytes = TR_BYTES_NONE;

	bytes = tr_sendb(peer, (void*)message, size);
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE)
		return bytes;
	
	this->AddSend(bytes);
	CcSocket::iOnSend.Execute(this);
	return bytes;
}
		
bool CcSocket::Close(void) {
	int fd_old = 0, fd_new = 0;
	
	fd_old = tr_check(this->_socket);
	if(fd_old > 0)
		fd_new = tr_close(CcSocket::_socket);
	if(fd_new > 0)
		CcLogErrorS("Cannot close socket " << this->_socket->fd);
	
	return(fd_new == 0 && fd_old > 0);
}

CcAddress CcSocket::GetLocal(void) {
	this->_semsocket.Wait();
	CcEndpoint ep;
	ep.Set(&CcSocket::_socket->local);
	CcAddress addr = ep.GetAddress();
	this->_semsocket.Post();
	return addr;
}

CcAddress CcSocket::GetRemote(void) {
	this->_semsocket.Wait();
	CcEndpoint ep;
	ep.Set(&CcSocket::_socket->remote);
	CcAddress addr = ep.GetAddress();
	this->_semsocket.Post();
	return addr;
}
		
int CcSocket::GetFID(void) {
	this->_semsocket.Wait();
	int fid = this->_socket->fd;
	this->_semsocket.Post();
	return fid;
}
		
CcStreamer* CcSocket::GetStream(int fid) {
	return this->_streams[fid];
}

bool CcSocket::AddStream(int fid) {
	this->_streams[fid] = new CcStreamer;
	return true;
}

bool CcSocket::RemStream(int fid) {
	CcStreamer* stream;
	stream = this->_streams[fid];
	this->_streams[fid] = NULL;
	this->_streams.erase(fid);
	
	if(stream != NULL)
		delete stream;
	return true;
}

bool CcSocket::HasStream(int fid) {
	CcStreamerMapIt it = this->_streams.find(fid);
	return(it != this->_streams.end());
}

tr_socket* CcSocket::GetPeer(int fid) {
	return this->_peers[fid];
}

bool CcSocket::AddPeer(tr_socket* peer) {
	this->_peers[peer->fd] = peer;
	return true;
}

bool CcSocket::RemPeer(int fid) {
	tr_socket* peer;
	peer = this->_peers[fid];
	this->_peers[fid] = NULL;
	this->_peers.erase(fid);
	
	if(peer != NULL)
		delete peer;
	return true;
}

bool CcSocket::HasPeer(int fid) {
	CcSocketMapIt it = this->_peers.find(fid);
	return(it != this->_peers.end());
}

CcAddress CcSocket::GetAddress(int fid) {
	CcEndpoint ep;
	tr_socket* sock = GetPeer(fid);
	ep.Set(&sock->remote);
	return ep.GetAddress();
}
		
tr_socket* CcSocket::GetPeer(CcAddress addr) {
	CcEndpoint cache;
	cache.SetAddress(addr);
	CcIp ip = cache.GetIp();
	CcPortUInt port = cache.GetPortUInt();

	CcSocketMapIt pit;
	tr_socket* peer = NULL;
	for(pit = this->_peers.begin(); pit != this->_peers.end(); pit++) {
		peer = pit->second;
		if(peer->remote.port == port) {
			if(strcmp(peer->remote.address, ip.c_str()) == 0)
				return peer;
		}
	}
	return NULL;
}

void CcSocket::Dump(void) {
	CcAddress addr;
	CcStreamerMapIt sit;
	CcSocketMapIt pit;
	
	printf("[CcSocket::Dump] Internal table:\n");
	printf("FID  Remote Address         Peer\n");
	int fid;
	for(sit = this->_streams.begin(); sit != this->_streams.end(); sit++) {
		fid = sit->first;
		addr = this->GetAddress(fid);
		pit = this->_peers.find(fid);
		
		printf("%4.4d ", fid);

		if(addr.empty() == false)
			printf("%-22.22s ", addr.c_str());
		else
			printf("%-22.22s ", "N/A");
		
		if(pit != this->_peers.end())
			printf("%p", pit->second);
		else
			printf("%s ", "NULL");

		printf("\n");
	}

}

CcAddress CcSocket::Lookup(std::string name) {
	char address[tr_getaddrlen()];
	tr_resolve(name.c_str(), address);
	return CcAddress(address);
}

#endif
