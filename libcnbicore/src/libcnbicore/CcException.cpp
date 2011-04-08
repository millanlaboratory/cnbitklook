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

#ifndef CCEXCEPTION_CPP
#define CCEXCEPTION_CPP

#include "CcException.hpp"

CcException::CcException(std::string info, std::string caller) {
	CcObject::SetName("CcException");
	this->_info = info;
	this->_caller = caller;
	this->CreateMessage();
}

CcException::~CcException(void) {
}

std::string CcException::Caller(void) const {
	return this->_caller;
}

std::string CcException::Info(void) const {
	return this->_info;
}

std::string CcException::Message(void) const {
	return this->_message;
}

void CcException::CreateMessage(void) {
	this->_message.append("[");
	this->_message.append(this->_caller);
	this->_message.append("] ");
	this->_message.append(this->_info);
}

std::ostream &operator<<(std::ostream& output, const CcException& obj) {
	output << "[CcException] " << obj.Message();
	return output;
}

std::ostream &operator<<(std::ostream& output, const CcException* obj) {
	output << "[CcException] " << obj->Message();
	return output;
}
	
bool CcException::operator==(const CcException& right) {
	return(this->_info.compare(right.Info()) == 0);
}

bool CcException::operator!=(const CcException& right) {
	return(this->_info.compare(right.Info()) != 0);
}

#endif
