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

#ifndef CLNMSSERVER_CPP 
#define CLNMSSERVER_CPP 

#include "ClNmsServer.hpp" 

void ClNmsServer::HandleRecvPeer(CcSocket* caller, CcAddress addr, 
		CcStreamer* stream) { 
	CcServer* server = (CcServer*)caller;
	std::string message;
	if(stream->Extract(&message, ClNmsLang::Hdr, ClNmsLang::Trl) == false)
		return;
	
	std::string luname, stname, stcontent;
	CcAddress luaddr;

	if(this->language.IsQuery(message.c_str(), &luname)) {
		if(this->language.CheckName(luname.c_str()) == true) { 
			if(this->Get(luname, &luaddr) == true) {
				CcLogInfoS("Query from " << addr << ": " 
						<< luname << "=" << luaddr);
				server->Send(language.Reply(luaddr), addr);
			} else {
				CcLogWarningS("Query from " << addr << ": " 
						<< luname << " NotFound");
				server->Send(language.Error(ClNmsLang::NotFound), addr);
			}
		} else {
			server->Send(language.Error(ClNmsLang::AlreadySet), addr);
			CcLogWarningS("Set from " << addr << ": " 
					<< luname << " AlreadySet");
		} 
	} else if(this->language.IsSet(message.c_str(), &luname, &luaddr)) {
		if(this->language.CheckName(luname.c_str()) == true) { 
			CcEndpoint lusolve(luaddr);
			CcEndpoint epsolve(addr);
			if(strcmp(lusolve.GetIp().c_str(), "0.0.0.0") == 0) {
				luaddr.assign(epsolve.GetIp());
				luaddr.append(":");
				luaddr.append(lusolve.GetPort());
				CcLogConfigS("Local endpoint " << lusolve.GetIp() <<
						" resolved as " << epsolve.GetIp());
			}
			if(this->Set(luname, luaddr) == true) {
				this->AddMonitored(luname, addr);
				server->Send(language.Ok(), addr);
				CcLogInfoS("Set from " << addr << ": " 
						<< luname << "=" << luaddr);
			} else {
				server->Send(language.Error(ClNmsLang::AlreadySet), addr);
				CcLogWarningS("Set from " << addr << ": " 
						<< luname << " AlreadySet");
			} 
		} else {
			server->Send(language.Error(ClNmsLang::NameFormatError), addr);
			CcLogWarningS("Set from " << addr << ": " 
					<< luname << " NameFormatError");
		}
	} else if(this->language.IsUnset(message.c_str(), &luname)) {
		if(this->language.CheckName(luname.c_str()) == true) { 
			if(this->Unset(luname) == true) {
				server->Send(language.Ok(), addr);
				CcLogInfoS("Unset from " << addr << ": " 
						<< luname);
			} else {
				server->Send(language.Error(ClNmsLang::NotFound), addr);
				CcLogWarningS("Unset from " << addr << ": " 
						<< luname << " NotFound");
			}
		} else {
			server->Send(language.Error(ClNmsLang::NameFormatError), addr);
			CcLogWarningS("Set from " << addr << ": " 
					<< luname << " NameFormatError");
		}
	} else if(this->language.IsStore(message.c_str(), &stname, &stcontent)) {
		if(this->Store(stname, stcontent) == true) {
			server->Send(language.Ok(), addr);
			CcLogInfoS("Store from " << addr << ": " 
					<< stname << ", " << stcontent.size() << " bytes");
		} else {
			server->Send(language.Error(ClNmsLang::AlreadyStored), addr);
			CcLogWarningS("Store from " << addr << ": " 
					<< stname << " AlreadyStored");
		}
	} else if(this->language.IsRetrieve(message.c_str(), &stname)) {
		if(this->Retrieve(stname, &stcontent) == true) {
			CcLogInfoS("Retrieve from " << addr << ": " 
					<< stname << ", " << stcontent.size() << " bytes");
			server->Send(language.Dispatch(stcontent), addr);
		} else {
			CcLogWarningS("Retrieve from " << addr << ": " 
					<< stname << " NotAvailable");
			server->Send(language.Error(ClNmsLang::NotAvailable), addr);
		}
	} else if(this->language.IsErase(message.c_str(), &stname)) {
		if(this->Erase(stname) == true) {
			server->Send(language.Ok(), addr);
			CcLogInfoS("Erase from " << addr << ": " 
					<< stname);
		} else {
			server->Send(language.Error(ClNmsLang::NotAvailable), addr);
			CcLogWarningS("Erase from " << addr << ": " 
					<< stname << " NotAvailable");
		}
	} else {
		server->Send(language.Error(ClNmsLang::NotUndestood), addr);
		CcLogWarningS("Message from " << addr << 
				" not understood: " << message); 
	}
}

void ClNmsServer::Register(CcServer* server) {
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnAcceptPeer, this, HandleAcceptPeer);
	CB_CcSocket(server->iOnDropPeer, this, HandleDropPeer);
	CB_CcSocket(server->iOnRecvPeer, this, HandleRecvPeer);
	this->_master = server;
}

bool ClNmsServer::Get(const std::string& name, CcAddress* address) {
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
		
bool ClNmsServer::Set(const std::string& name, const CcAddress address) {
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

bool ClNmsServer::Unset(const std::string& name) {
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
		
void ClNmsServer::Main(void) {
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
			CcLogWarningS("Endpoint " << address << 
					" dropped: unsetting " << name);
			this->_monitor.erase(name);
			this->Unset(name);
		}
		this->_semmonitor.Post();

	}
}

void ClNmsServer::StartMonitor(double waitms) {
	if(waitms <= 0.00f)
		return;

	CcLogConfigS("Starting monitor: " << waitms << "ms");
	this->_monitorms = waitms;
	CcThread::Start();
}

void ClNmsServer::StopMonitor(void) {
	CcLogConfig("Stopping monitor");
	CcThread::Stop();
}

bool ClNmsServer::IsMonitoring(void) {
	return CcThread::IsRunning();
}
		
void ClNmsServer::AddMonitored(const std::string& name, CcAddress address) {
	this->_semmonitor.Wait();
	this->_monitor[name] = address;
	this->_semmonitor.Post();
}

bool ClNmsServer::Retrieve(const std::string& name, std::string* content) {
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
		
bool ClNmsServer::Store(const std::string& name, 
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

bool ClNmsServer::Erase(const std::string& name) {
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
