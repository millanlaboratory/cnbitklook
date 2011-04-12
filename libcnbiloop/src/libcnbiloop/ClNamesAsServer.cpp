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

#ifndef CLNAMESASSERVER_CPP 
#define CLNAMESASSERVER_CPP 

#include "ClNamesAsServer.hpp" 

void ClNamesAsServer::HandleBind(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogConfigS(this->_stream, "Bound TCP socket: " << server->GetLocal());
}

void ClNamesAsServer::HandleRelease(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogConfigS(this->_stream, "Released TCP socket: " << server->GetLocal());
}

void ClNamesAsServer::HandleListen(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogConfigS(this->_stream, 
			"Listening on TCP socket: " << server->GetLocal());
}

void ClNamesAsServer::HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogConfigS(this->_stream, "Accepted TCP endpoint: " << address);
}

void ClNamesAsServer::HandleDropEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogConfigS(this->_stream, "Dropped TCP endpoint: " << address);
}

void ClNamesAsServer::HandleRecvEndpoint(CcSocket* caller, CcAddress address) { 
	CcServerMulti* server = (CcServerMulti*)caller;
	std::string message;
	if(server->datastreams[address]->Extract(&message, 
				ClNamesLang::Hdr, ClNamesLang::Trl,
				CcStreamer::Forward) == false)
		return;
	
	std::string luname, stname, stcontent;
	CcAddress luaddr;

	if(this->language.IsQuery(message.c_str(), &luname)) {
		if(this->Get(luname, &luaddr) == true) {
			CcLogInfoS(this->_stream, "Query from " << address << ": " 
					<< luname << "=" << luaddr);
			server->Send(language.Reply(luaddr), address);
		} else {
			CcLogWarningS(this->_stream, "Query from " << address << ": " 
					<< luname << " NotFound");
			server->Send(language.Error(ClNamesLang::NotFound), address);
		}
	} else if(this->language.IsSet(message.c_str(), &luname, &luaddr)) {
		CcEndpoint lusolve(luaddr);
		CcEndpoint epsolve(address);
		if(strcmp(lusolve.GetIp().c_str(), "0.0.0.0") == 0) {
			luaddr.assign(epsolve.GetIp());
			luaddr.append(":");
			luaddr.append(lusolve.GetPort());
			CcLogConfigS(this->_stream, "Local endpoint " << lusolve.GetIp() <<
					" resolved as " << epsolve.GetIp());
		}
		if(this->Set(luname, luaddr) == true) {
			this->AddMonitored(luname, address);
			server->Send(language.Ok(), address);
			CcLogInfoS(this->_stream, "Set from " << address << ": " 
					<< luname << "=" << luaddr);
		} else {
			server->Send(language.Error(ClNamesLang::AlreadySet), address);
			CcLogWarningS(this->_stream, "Set from " << address << ": " 
					<< luname << " AlreadySet");
		}
	} else if(this->language.IsUnset(message.c_str(), &luname)) {
		if(this->Unset(luname) == true) {
			server->Send(language.Ok(), address);
			CcLogInfoS(this->_stream, "Unset from " << address << ": " 
					<< luname);
		} else {
			server->Send(language.Error(ClNamesLang::NotFound), address);
			CcLogWarningS(this->_stream, "Unset from " << address << ": " 
					<< luname << " NotFound");
		}
	} else if(this->language.IsStore(message.c_str(), &stname, &stcontent)) {
		if(this->Store(stname, stcontent) == true) {
			server->Send(language.Ok(), address);
			CcLogInfoS(this->_stream, "Store from " << address << ": " 
					<< stname << ", " << stcontent.size() << " bytes");
		} else {
			server->Send(language.Error(ClNamesLang::AlreadyStored), address);
			CcLogWarningS(this->_stream, "Store from " << address << ": " 
					<< stname << " AlreadyStored");
		}
	} else if(this->language.IsRetrieve(message.c_str(), &stname)) {
		if(this->Retrieve(stname, &stcontent) == true) {
			CcLogInfoS(this->_stream, "Retrieve from " << address << ": " 
					<< stname << ", " << stcontent.size() << " bytes");
			server->Send(language.Dispatch(stcontent), address);
		} else {
			CcLogWarningS(this->_stream, "Retrieve from " << address << ": " 
					<< luname << " NotAvailable");
			server->Send(language.Error(ClNamesLang::NotAvailable), address);
		}
	} else if(this->language.IsErase(message.c_str(), &stname)) {
		if(this->Erase(stname) == true) {
			server->Send(language.Ok(), address);
			CcLogInfoS(this->_stream, "Erase from " << address << ": " 
					<< stname);
		} else {
			server->Send(language.Error(ClNamesLang::NotAvailable), address);
			CcLogWarningS(this->_stream, "Erase from " << address << ": " 
					<< stname << " NotAvailable");
		}
	} else {
		server->Send(language.Error(ClNamesLang::NotUndestood), address);
		CcLogWarningS(this->_stream, "Message from " << address << 
				" not understood: " << message); 
	}
}

void ClNamesAsServer::Register(CcServerMulti* server) {
	CB_CcSocket(server->iOnBind, this, HandleBind);
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnListen, this, HandleListen);
	CB_CcSocket(server->iOnAcceptEndpoint, this, HandleAcceptEndpoint);
	CB_CcSocket(server->iOnDropEndpoint, this, HandleDropEndpoint);
	CB_CcSocket(server->iOnRecvEndpoint, this, HandleRecvEndpoint);
	this->_master = server;
}

bool ClNamesAsServer::Get(const std::string& name, CcAddress* address) {
	this->_semlookup.Wait();
	std::map<CcAddress, std::string>::iterator it;
	it = this->_lookup.find(name);
	if(it == this->_lookup.end()) {
		this->_semlookup.Post();
		return false;
	}
	
	address->assign((*it).second);
	this->_semlookup.Post();
	return true;
}
		
bool ClNamesAsServer::Set(const std::string& name, const CcAddress address) {
	this->_semlookup.Wait();
	std::map<CcAddress, std::string>::iterator it;
	it = this->_lookup.find(name);
	if(it != this->_lookup.end()) {
		this->_semlookup.Post();
		return false;
	}
	
	this->_lookup[name] = address;
	this->_semlookup.Post();
	return true;
}

bool ClNamesAsServer::Unset(const std::string& name) {
	this->_semlookup.Wait();
	std::map<CcAddress, std::string>::iterator it;
	it = this->_lookup.find(name);
	if(it == this->_lookup.end()) {
		this->_semlookup.Post();
		return false;
	}

	this->_lookup.erase(it);
	this->_semlookup.Post();
	return true;
}
		
void ClNamesAsServer::Main(void) {
	if(CcThread::IsRunning() == false)
		return;
	
	CcAddress address;
	std::string name;
	std::map<CcAddress, std::string>::iterator it;

	while(CcThread::IsRunning() == true) {
		CcTime::Sleep(this->_monitorms);

		CcLogDebug("Checking lookup...");
		this->_semmonitor.Wait();
		for(it = this->_monitor.begin(); it != this->_monitor.end(); it++) {
			name.assign((*it).first);
			address.assign((*it).second);
			if(this->_master->Send("", address) >= TR_BYTES_NONE)
				continue;
			CcLogWarningS(this->_stream, "Endpoint " << address << 
					" dropped: unsetting " << name);
			this->_monitor.erase(name);
			this->Unset(name);
		}
		this->_semmonitor.Post();

	}
}

void ClNamesAsServer::StartMonitor(double waitms) {
	if(waitms <= 0.00f)
		return;

	CcLogConfigS(this->_stream, "Starting monitor: " << waitms << "ms");
	this->_monitorms = waitms;
	CcThread::Start();
}

void ClNamesAsServer::StopMonitor(void) {
	CcLogConfig("Stopping monitor");
	CcThread::Stop();
}

bool ClNamesAsServer::IsMonitoring(void) {
	return CcThread::IsRunning();
}
		
void ClNamesAsServer::AddMonitored(const std::string& name, CcAddress address) {
	this->_semmonitor.Wait();
	this->_monitor[name] = address;
	this->_semmonitor.Post();
}

bool ClNamesAsServer::Retrieve(const std::string& name, std::string* content) {
	this->_semstorage.Wait();
	std::map<CcAddress, std::string>::iterator it;
	it = this->_storage.find(name);
	if(it == this->_storage.end()) {
		this->_semstorage.Post();
		return false;
	}
	
	content->assign((*it).second);
	this->_semstorage.Post();
	return true;
}
		
bool ClNamesAsServer::Store(const std::string& name, 
		const std::string& content) {
	this->_semstorage.Wait();
	std::map<CcAddress, std::string>::iterator it;
	it = this->_storage.find(name);
	if(it != this->_storage.end()) {
		this->_semstorage.Post();
		return false;
	}
	
	this->_storage[name] = content;
	this->_semstorage.Post();
	return true;
}

bool ClNamesAsServer::Erase(const std::string& name) {
	this->_semstorage.Wait();
	std::map<std::string, std::string>::iterator it;
	it = this->_storage.find(name);
	if(it == this->_storage.end()) {
		this->_semstorage.Post();
		return false;
	}

	this->_storage.erase(it);
	this->_semstorage.Post();
	return true;
}
		
#endif
