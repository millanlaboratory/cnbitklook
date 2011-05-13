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
#include "CcTime.hpp"
#include "CcFlags.hpp"
#include "CcException.hpp"
#include "CcLog.hpp"
#include <libtransport/tr_tcp.h>
#include <stdio.h>

CcServer::CcServer(CcServerRole role, unsigned int bsize, unsigned int maxconns)
	: CcSocket(bsize, maxconns) {
	this->_role.Set(role);
}

CcServer::~CcServer(void) {
	this->Release();
}

void CcServer::OpenSocket(void) {
	int status;
	this->_semsocket.Wait();
	tr_tcpserver(CcSocket::_socket);
	status = tr_open(CcSocket::_socket);
	this->_semsocket.Post();

	if(status == -1)
		CcThrow("Can not open socket");
}

void CcServer::CloseSocket(void) {
	int status;
	this->_semsocket.Wait();
	status = tr_close(CcSocket::_socket);
	this->_semsocket.Post();

	if(status > 0)
		CcThrow("Can not close socket");
}
		
void CcServer::Bind(const CcEndpoint enpoint, const unsigned int wait) {
	this->Bind(enpoint.GetPortUInt(), wait);
}
		
void CcServer::Bind(const CcAddress address, const unsigned int wait) {
	CcEndpoint cache(address);
	this->Bind(cache.GetPortUInt(), wait);
}

void CcServer::Bind(const unsigned int port, const unsigned int wait) {
	if(this->_role.Get() == CcServer::AsEndpoint)
		CcThrow("Can not bind socket AsServer");

	if(CcThread::IsRunning() == true)
		return;
	
	this->OpenSocket();

	unsigned int waited = 0;
	char sport[5];
	sprintf(sport, "%u", port);

	int status;
	while(waited <= wait) {
		CcSocket::_semsocket.Wait();
		status = tr_bind(CcSocket::_socket, sport);
		this->_hostLocal.Set(&(CcSocket::_socket->local));
		CcSocket::_semsocket.Post();

		if(status == 0) 
			break;
		sleep(1);
		++waited;
	}

	if(status < 0)
		CcThrow("Can not bind socket");

	this->pOnBind();
	this->iOnBind.Execute((CcSocket*)this);
	
	CcThread::Start();
	while(CcThread::IsRunning() == false) 
		CcTime::Sleep(CCASYNC_WAIT_CONNECT);
	this->Listen();
}
		
void CcServer::Bind(const tr_socket* const master) {
	if(this->_role.Get() == CcServer::AsServer)
		CcThrow("Can not accept AsEndpoint");
}

void CcServer::Release(void) {
	if(CcThread::IsRunning() == false)
		return;
	
	CcThread::Stop();
	CcThread::Join();
	
	this->CloseSocket();

	this->pOnRelease();
	this->iOnRelease.Execute((CcSocket*)this);
}

void CcServer::Listen(void) {
	CcSocket::_semsocket.Wait();
	int status = tr_listen(CcSocket::_socket);
	CcSocket::_semsocket.Post();

	if(status == -1)
		CcThrow("Can not listen");
	
	this->pOnListen();
	this->iOnListen.Execute((CcSocket*)this);
}

CcAddress CcServer::GetLocal(void) {
	CcSocket::_semsocket.Wait();
	CcAddress id = this->_hostLocal.GetAddress();
	CcSocket::_semsocket.Post();
	return id;
}

#endif
