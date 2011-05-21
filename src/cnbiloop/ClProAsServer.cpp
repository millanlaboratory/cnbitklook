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

#ifndef CLPROASSERVER_CPP 
#define CLPROASSERVER_CPP 

#include "ClProAsServer.hpp" 

void ClProAsServer::HandleRecvPeer(CcSocket* caller, CcAddress addr, 
		CcStreamer* stream) { 
	CcServer* server = (CcServer*)caller;
	
	std::string message;
	if(stream->Extract(&message, ClProLang::Hdr, ClProLang::Trl) == false)
		return;

	int pid;
	std::string function, path0, path1;
	std::string pipename, addressD, addressC;
	
	if(this->language.IsFork(message.c_str())) {
		pid = this->Fork();
		if(pid > 0) {
			server->Send(language.Ok(pid), addr);
			CcLogInfoS("Fork from " << addr << ": " << pid);
		} else {
			server->Send(language.Error(ClProLang::ForkFailed), addr);
			CcLogWarningS("Fork from " << addr << ": " << pid << " ForkFailed");
		}
	} else if(this->language.IsExec(message.c_str(), &pid, &function)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), addr);
			CcLogWarningS("Exec from " << addr << ": " 
					<< pid << ", " << function << " NotFound");
		} else {
			function.append("\n");
			process->Exec(function);
			server->Send(language.Ok(pid), addr);
			CcLogInfoS("Exec from " << addr << ": " << pid << " " << function);
		}
	} else if(this->language.IsTerminate(message.c_str(), &pid)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), addr);
			CcLogWarningS("Terminate from " << addr << ": " 
					<< pid << " NotFound");
		} else {
			process->Terminate();
			this->Remove(pid);
			server->Send(language.Ok(pid), addr);
			CcLogInfoS("Terminate from " << addr << ": " << pid);
		}
	} else if(this->language.IsIsAlive(message.c_str(), &pid)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), addr);
				CcLogWarningS("IsAlive from " << addr << ": " 
						<< pid << " NotFound");
		} else {
			if(process->IsAlive()) {
				server->Send(language.Ok(pid), addr);
			} else {
				server->Send(language.Error(ClProLang::IsDead), addr);
				char buffer[CCCORE_1MB];
				process->Read(buffer, CCCORE_1MB);
				CcLogWarningS("IsAlive from " << addr << ": " 
						<< pid << " IsDead, " << buffer);
			}
		}
	} else if(this->language.IsDirectory(message.c_str(), &pid, &path0)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), addr);
			CcLogWarningS("Directory from " << addr << ":" 
					" PID=" << pid << ", " << path0 << " NotFound");
		} else {
			process->Directory(path0);
			server->Send(language.Ok(pid), addr);
			CcLogInfoS("Directory from " << addr << ":" 
					" PID=" << pid << " " << path0);
		}
	} else if(this->language.IsInclude(message.c_str(), &pid, &path0)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), addr);
			CcLogWarningS("Include from " << addr << ":" 
					" PID=" << pid  << path0 << " NotFound");
		} else {
			process->Include(path0);
			server->Send(language.Ok(pid), addr);
			CcLogInfoS("Include from " << addr << ": " 
					" PID=" << pid << " " << path0);
		}
	} else if(this->language.IsInclude(message.c_str(), &pid, &path0, &path1)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), addr);
			CcLogWarningS("Include from " << addr << ":" 
					" PID=" << pid  << path0 << ", " << path1 << " NotFound");
		} else {
			process->Include(path0, path1);
			server->Send(language.Ok(pid), addr);
			CcLogInfoS("Include from " << addr << ":" 
					" PID=" << pid << " " << path0 << ", " << path1);
		}
	} else {
		CcLogWarningS("Message from " << addr << 
				" not understood: " << message); 
		server->Send(language.Error(ClProLang::NotUndestood), addr);
	}
}

void ClProAsServer::Register(CcServer* server) {
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnAcceptPeer, this, HandleAcceptPeer);
	CB_CcSocket(server->iOnDropPeer, this, HandleDropPeer);
	CB_CcSocket(server->iOnRecvPeer, this, HandleRecvPeer);
}

int ClProAsServer::Fork(void) {
	int pid = 0;
	ClMatlab* process = new ClMatlab();
	pid = process->Fork();
	this->_sempool.Wait();
	this->_pool[pid] = process;
	this->_sempool.Post();
	
	return pid;
}

void ClProAsServer::Remove(int pid) {
	this->_sempool.Wait();
	this->_pool.erase(pid);
	this->_sempool.Post();
}

ClMatlab* ClProAsServer::Get(int pid) {
	this->_sempool.Wait();
	std::map<int, ClMatlab*>::iterator it;
	it = this->_pool.find(pid);
	if(it == this->_pool.end())
		return NULL;
	
	ClMatlab* process = (*it).second;
	this->_sempool.Post();
	return process;
}
#endif
