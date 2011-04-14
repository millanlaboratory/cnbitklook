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

#ifndef CLACQCLIENT_CPP 
#define CLACQCLIENT_CPP 

#include "ClAcqClient.hpp" 

bool ClAcqClient::Connect(const CcAddress address) {
	return ClClient::Connect(address);
}

int ClAcqClient::AddLabelGDF(const GDFEvent label) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.AddLabelGDF(label);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClAcqLang::Hdr, ClAcqLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClAcqLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClAcqLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClAcqLang::StatusLost;
}

int ClAcqClient::AddLabelLPT(const HWTrigger label) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.AddLabelLPT(label);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClAcqLang::Hdr, ClAcqLang::Trl, ClClient::_waitms);
	
	if(status == false)
		return ClAcqLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClAcqLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClAcqLang::StatusLost;
}

int ClAcqClient::AddLabelTXT(const std::string& label) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.AddLabelTXT(label);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClAcqLang::Hdr, ClAcqLang::Trl, ClClient::_waitms);
	
	if(status == false)
		return ClAcqLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClAcqLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClAcqLang::StatusLost;
}
		
int ClAcqClient::OpenXDF(const std::string& filegdf, 
		const std::string& filelog, const std::string& linelog) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.OpenXDF(filegdf, filelog, linelog);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClAcqLang::Hdr, ClAcqLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClAcqLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClAcqLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClAcqLang::StatusLost;
}

int ClAcqClient::CloseXDF(void) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.CloseXDF();
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClAcqLang::Hdr, ClAcqLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClAcqLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClAcqLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClAcqLang::StatusLost;
}

#endif
