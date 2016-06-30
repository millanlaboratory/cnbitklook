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

#ifndef CLNMSCLIENT_CPP 
#define CLNMSCLIENT_CPP 

#include "ClNmsClient.hpp" 

bool ClNmsClient::Connect(const CcAddress address) {
	return ClClient::Connect(address);
}

int ClNmsClient::Query(const std::string& name, CcAddress* address) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Query(name);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNmsLang::Hdr, ClNmsLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClNmsLang::NoReply;

	if(this->_language.IsReply(reply.c_str(), address))
		return ClNmsLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid)) {
		if(errorid == ClNmsLang::NameFormatError) 
			CcLogError("Port name must be /PORT");
		return errorid;
	} else return ClNmsLang::StatusLost;
}

int ClNmsClient::Set(const std::string& name, CcAddress address) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Set(name, address);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNmsLang::Hdr, ClNmsLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClNmsLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClNmsLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid)) {
		if(errorid == ClNmsLang::NameFormatError) 
			CcLogError("Port name must be /PORT");
		return errorid;
	} else
		return ClNmsLang::StatusLost;
}

int ClNmsClient::Unset(const std::string& name) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Unset(name);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNmsLang::Hdr, ClNmsLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClNmsLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClNmsLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid)) {
		if(errorid == ClNmsLang::NameFormatError) 
			CcLogError("Port name must be /PORT");
		return errorid;
	} else
		return ClNmsLang::StatusLost;
}

		
CcAddress ClNmsClient::Query(const std::string& name) {
	CcAddress address;
	if(this->Query(name, &address) != ClNmsLang::Successful)
		address.assign("");
	return address;
}

int ClNmsClient::Retrieve(const std::string& name, std::string* content) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Retrieve(name);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNmsLang::Hdr, ClNmsLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClNmsLang::NoReply;

	if(this->_language.IsDispatch(reply.c_str(), content))
		return ClNmsLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClNmsLang::StatusLost;
}

int ClNmsClient::Store(const std::string& name, const std::string& content) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Store(name, content);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNmsLang::Hdr, ClNmsLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClNmsLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClNmsLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClNmsLang::StatusLost;
}

int ClNmsClient::Erase(const std::string& name) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Erase(name);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClNmsLang::Hdr, ClNmsLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClNmsLang::NoReply;

	if(this->_language.IsOk(reply.c_str()))
		return ClNmsLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClNmsLang::StatusLost;
}
		
bool ClNmsClient::Store(const std::string& name, CcFile* file) {
	return(this->Store(name, file->Get()) == ClNmsLang::Successful);
}

bool ClNmsClient::Retrieve(const std::string& name, CcFile* file) {
	std::string content;
	int status = this->Retrieve(name, &content);
	if(status != ClNmsLang::Successful)
		return false;
	file->Set(content);
	return true;
}

bool ClNmsClient::StoreFile(const std::string& name, 
		const std::string& filename) {
	CcFile file;
	if(file.Load(filename) == false)
		return false;
	return this->Store(name, &file);
}

bool ClNmsClient::RetrieveFile(const std::string& name, 
		const std::string& filename) {
	CcFile file;
	if(this->Retrieve(name, &file) == false) 
		return false;
	return(file.Save(filename));
}

bool ClNmsClient::StoreConfig(const std::string& component, 
		const std::string& name, const std::string& content) {
	std::string tname = component + "." + name;
	return(this->Store(tname, content) == ClNmsLang::Successful);
}

std::string ClNmsClient::RetrieveConfig(const std::string& component, 
		const std::string& name) {
	std::string tname = component + "." + name;
	std::string content;
	if(this->Retrieve(tname, &content) != ClNmsLang::Successful)
		content.clear();
	return content;
}

bool ClNmsClient::EraseConfig(const std::string& component, 
		const std::string& name) {
	std::string tname = component + "." + name;
	return(this->Erase(tname) == ClNmsLang::Successful);
}
#endif
