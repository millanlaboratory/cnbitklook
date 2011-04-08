/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLNAMESCLIENT_CPP 
#define CLNAMESCLIENT_CPP 

#include "ClNamesClient.hpp" 

bool ClNamesClient::Connect(const CcAddress address) {
	return ClClient::Connect(address);
}

int ClNamesClient::Query(const std::string& name, CcAddress* address) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Query(name);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNamesLang::Hdr, ClNamesLang::Trl, ClClient::_waitms);

	if(status == false) {
		CcLogWarningS(ClClient::_stream, "No reply in " << ClClient::_waitms << "ms");
		return ClNamesLang::NoReply;
	}

	if(this->_language.IsReply(reply.c_str(), address))
		return ClNamesLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClNamesLang::StatusLost;
}

int ClNamesClient::Set(const std::string& name, CcAddress address) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Set(name, address);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNamesLang::Hdr, ClNamesLang::Trl, ClClient::_waitms);

	if(status == false) {
		CcLogWarningS(ClClient::_stream, "No reply in " << ClClient::_waitms << "ms");
		return ClNamesLang::NoReply;
	}

	if(this->_language.IsOk(reply.c_str()))
		return ClNamesLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClNamesLang::StatusLost;
}

int ClNamesClient::Unset(const std::string& name) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Unset(name);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNamesLang::Hdr, ClNamesLang::Trl, ClClient::_waitms);

	if(status == false) {
		CcLogWarningS(ClClient::_stream, "No reply in " << ClClient::_waitms << "ms");
		return ClNamesLang::NoReply;
	}

	if(this->_language.IsOk(reply.c_str()))
		return ClNamesLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClNamesLang::StatusLost;
}

		
CcAddress ClNamesClient::Query(const std::string& name) {
	CcAddress address;
	if(this->Query(name, &address) != ClNamesLang::Successful)
		address.assign("");
	return address;
}

#endif
