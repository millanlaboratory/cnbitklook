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

#ifndef CCSOCKET_CPP
#define CCSOCKET_CPP

#include "CcSocket.hpp"
#include "CcBasic.hpp"
#include <string.h>

const CcHostname CcSocket::HostnameUnset = "none";
const CcPort CcSocket::PortUnset = "0";
const CcIp CcSocket::IpUnset = "0.0.0.0";
const CcAddress CcSocket::AddressUnset = "0.0.0.0:0";

CcSocket::CcSocket(unsigned int bsize, unsigned int maxconn) : 
	CcObject("CcSocket") {
	this->_semsocket.Wait();
	this->_socket = new tr_socket;
	tr_init_socket(this->_socket, bsize, maxconn);
	this->_semsocket.Post();

	this->_sembuffer.Wait();
	this->_buffer = NULL;
	this->_sembuffer.Post();
	
	this->AllocBuffer();
}

CcSocket::~CcSocket(void) {
	this->_semsocket.Wait();
	if(tr_check(this->_socket))
		tr_close(this->_socket);
	tr_free(this->_socket);
	delete this->_socket;
	this->_semsocket.Post();

	this->FreeBuffer();
}
		
void CcSocket::AllocBuffer(void) {
	this->_sembuffer.Wait();
	this->_buffer = new char[this->_socket->bsize];
	memset(this->_buffer, 0, this->_socket->bsize * sizeof(const char));
	this->_sembuffer.Post();
}

void CcSocket::FreeBuffer(void) {
	this->_sembuffer.Wait();
	if(this->_buffer != NULL) {
		delete [] this->_buffer;
		this->_buffer = NULL;
	}
	this->_sembuffer.Post();
}

void CcSocket::AddBytesSend(const int bytes) {
	if(bytes <= 0) 
		return;

	this->_mtxbytes.Lock();
	this->_bytesSend += bytes;
	this->_mtxbytes.Release();
}

void CcSocket::AddBytesRecv(const int bytes) {
	if(bytes <= 0)
		return;
	
	this->_mtxbytes.Lock();
	this->_bytesRecv += bytes;
	this->_mtxbytes.Release();
}

long unsigned int CcSocket::GetBytesSend(void) {
	this->_mtxbytes.Lock();
	long unsigned int bytes = this->_bytesSend;
	this->_mtxbytes.Release();
	
	return bytes;
}

long unsigned int CcSocket::GetBytesRecv(void) {
	this->_mtxbytes.Lock();
	long unsigned int bytes = this->_bytesRecv;
	this->_mtxbytes.Release();
	
	return bytes;
}

bool CcSocket::IsConnected(void) {
	this->_semsocket.Wait();
	bool status = (tr_connected(this->_socket) == 0);
	this->_semsocket.Post();
	return status;
}

#endif


/*
static void ChopMessage(const char* buffer, unsigned int rep = 2);
static void ZeroMessage(const char* buffer, unsigned int buflen);
static bool EmptyMessage(const char* buffer);
*/

/*
void CcSocket::ChopMessage(const char* buffer, unsigned int rep) {
	if(buffer = NULL) 
		throw CcException("Buffer not allocated", __PRETTY_FUNCTION__);

	int len = strlen((char*)buffer);
	for(unsigned int j = 0; j < rep; j++)
		if(buffer[len - j] == '\n' && j >= 0)
			buffer[len - j] = '\0';
}

void CcSocket::ZeroMessage(const char* buffer, unsigned int buflen) {
	if(buffer = NULL) 
		throw CcException("Buffer not allocated", __PRETTY_FUNCTION__);
	
	memset(buffer, 0, buflen * sizeof(char));
}

bool CcSocket::EmptyMessage(const char* buffer) {
	if(buffer = NULL) 
		throw CcException("Buffer not allocated", __PRETTY_FUNCTION__);
	
	if(buffer[1] != '\n' && buffer[1] != '\0')
		return false;
	return true;
}
*/
