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

#ifndef CLPROCLIENT_CPP 
#define CLPROCLIENT_CPP 

#include "ClProClient.hpp" 

bool ClProClient::Connect(const CcAddress address) {
	return ClClient::Connect(address);
}

int ClProClient::Fork(int* rpid) {
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Fork();
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClProLang::Hdr, ClProLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClProLang::NoReply;

	if(this->_language.IsOk(reply.c_str(), rpid))
		return ClProLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid)) 
		return errorid;
	else
		return ClProLang::StatusLost;
}
		
int ClProClient::ForkAndCheck(int* rpid, double waitms) {
	int status = this->Fork(rpid);
	if(status != ClProLang::Successful)
		return status;

	CcTimeValue tic;
	CcTime::Tic(&tic);
	while(CcTime::Toc(&tic) < waitms) {
		status = this->IsAlive(*rpid);
		switch(status) {
			case ClProLang::Successful:
			case ClProLang::IsDead:
				return status;
				break;
		}
		CcTime::Sleep(250.00f);
	}
	return status;
}
		
int ClProClient::Launch(const int pid, const std::string& call) {
	int rpid = 0;
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Launch(pid, call);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClProLang::Hdr, ClProLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClProLang::NoReply;

	if(this->_language.IsOk(reply.c_str(), &rpid))
		return ClProLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid)) 
		return errorid;
	else
		return ClProLang::StatusLost;
}

int ClProClient::IsAlive(const int pid) {
	int rpid = 0;
	int errorid = 0;
	std::string message, reply;
	
	this->_language.IsAlive(pid);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClProLang::Hdr, ClProLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClProLang::NoReply;

	if(this->_language.IsOk(reply.c_str(), &rpid))
		return ClProLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid)) 
		return errorid;
	else
		return ClProLang::StatusLost;
}


int ClProClient::Terminate(const int pid) {
	int rpid = 0;
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Terminate(pid);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClProLang::Hdr, ClProLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClProLang::NoReply;

	if(this->_language.IsOk(reply.c_str(), &rpid))
		return ClProLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClProLang::StatusLost;
}

int ClProClient::ChangeDirectory(const int pid, const std::string& path) {
	int rpid = 0;
	int errorid = 0;
	std::string message, reply;
	
	this->_language.ChangeDirectory(pid, path);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClProLang::Hdr, ClProLang::Trl, ClClient::_waitms);

	if(status == false) 
		return ClProLang::NoReply;

	if(this->_language.IsOk(reply.c_str(), &rpid))
		return ClProLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClProLang::StatusLost;
}

int ClProClient::Include(const int pid, const std::string& path0, 
	const std::string& path1) {
	int rpid = 0;
	int errorid = 0;
	std::string message, reply;
	
	this->_language.Include(pid, path0, path1);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClProLang::Hdr, ClProLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClProLang::NoReply;

	if(this->_language.IsOk(reply.c_str(), &rpid))
		return ClProLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClProLang::StatusLost;
}

int ClProClient::LaunchNDF(const int pid, const std::string& function, 
	const std::string& pipename, const CcAddress addressD, 
	const CcAddress addressC, const std::string& extra) {
	int rpid = 0;
	int errorid = 0;
	std::string message, reply;
	
	this->_language.LaunchNDF(pid, function, pipename, addressD, addressC,
			extra);
	bool status = ClClient::_client.SendRecv(this->_language.message->buffer,
			&reply, ClProLang::Hdr, ClProLang::Trl, ClClient::_waitms);

	if(status == false)
		return ClProLang::NoReply;

	if(this->_language.IsOk(reply.c_str(), &rpid))
		return ClProLang::Successful;
	else if(this->_language.IsError(reply.c_str(), &errorid))
		return errorid;
	else
		return ClProLang::StatusLost;
}

int ClProClient::IncludeNDF(const int pid) {
	return this->Include(pid, "$CNBITK_MATLAB/", "/opt/cnbitkmat/");
}

bool ClProClient::Check(const int pid, double waitms) {
	CcTimeValue tic;
	CcTime::Tic(&tic);
	int status = ClProLang::IsDead;
	while(CcTime::Toc(&tic) <= waitms) {
		status = this->IsAlive(pid);
		if(status != ClProLang::Successful)
			return false;
		CcTime::Sleep(5.00f);
	}
	return true;
}
		
#endif
