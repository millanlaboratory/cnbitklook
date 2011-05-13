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

#ifndef CCHOST_CPP
#define CCHOST_CPP

#include "CcEndpoint.hpp"
#include "CcSocket.hpp"
#include "CcBasic.hpp"
#include <libtransport/tr_names.h>
#include <sstream>
#include <string.h>

CcEndpoint::CcEndpoint(void) {
}

CcEndpoint::CcEndpoint(const CcIp ip, const CcPort port) {
	this->SetIp(ip);
	this->SetPort(port);
}

CcEndpoint::CcEndpoint(const CcIp ip, const unsigned int port) {
	this->SetIp(ip);
	this->SetPortUInt(port);
}
		
CcEndpoint::CcEndpoint(const CcAddress address) {
	this->SetAddress(address);
}

CcEndpoint::~CcEndpoint(void) {
}

void CcEndpoint::Clear(void) {
	this->_ip = CcSocket::IpUnset;
	this->_port = CcSocket::PortUnset;
	this->_iport = CcSocket::PortUintUnset;
	this->_address = CcSocket::AddressUnset;
}

void CcEndpoint::SetIp(const CcIp ip) {
	if(ip.size() > tr_getaddrlen())
		CcThrow("Address exceeds predefined size");
	this->_ip = ip;
	this->MakeAddress(this->_ip, this->_iport);
}

void CcEndpoint::SetPortUInt(const unsigned int port) {
	this->_iport = port;
	this->MakeAddress(this->_ip, this->_iport);
	// TODO create an ultimate way to perform conversions
	char buffer[5];
	sprintf(buffer, "%d", port);
	this->_port.assign(buffer);
}

CcIp CcEndpoint::GetIp(void) const {
	return this->_ip;
}

unsigned int CcEndpoint::GetPortUInt(void) const {
	return this->_iport;
}

void CcEndpoint::SetPort(const CcPort port) {
	this->_port = port;
	this->MakeAddress(this->_ip, this->_port);
	// TODO create an ultimate way to perform conversions
	sscanf(port.c_str(), "%d", &this->_iport);
}

CcPort CcEndpoint::GetPort(void) const {
	return this->_port;
}

void CcEndpoint::Set(const tr_host *host, bool fixlocal) {
	if(strcmp(host->address, "0.0.0.0") == 0 && fixlocal)
		this->_ip.assign("127.0.0.1");
	else
		this->_ip.assign(host->address);
	this->_iport = host->port;

	this->MakeAddress(this->_ip, this->_iport);
}
		
void CcEndpoint::SetAddress(const CcAddress address) {
	this->_address = address;
	CcEndpoint::DecomposeAddress(this->_address);
}

CcAddress CcEndpoint::GetAddress(void) const {
	return this->_address;
}

void CcEndpoint::MakeAddress(const CcIp ip, const CcPort port) {
	this->_address.clear();
	this->_address.append(ip);
	this->_address.append(":");
	this->_address.append(port);
}

void CcEndpoint::MakeAddress(const CcIp ip, const unsigned int port) {
	std::stringstream stream;
	stream << ip;
	stream << ":";
	stream << port;
	this->_address = stream.str();
}
		
void CcEndpoint::MakeAddress(const tr_host *host) {
	CcIp ip(host->address);
	this->MakeAddress(ip, host->port);
}
		
void CcEndpoint::DecomposeAddress(const CcAddress address) {
	char cip[32];
	if(address.find(":") != std::string::npos) {
		sscanf(address.c_str(), "%[^':']:%u", cip, &this->_iport);
		this->_ip.assign(cip);
	} else {
		sscanf(address.c_str(), "%u", &this->_iport);
	}

	std::stringstream stream;
	stream << this->_iport;
	this->_port.assign(stream.str());
}

#endif
