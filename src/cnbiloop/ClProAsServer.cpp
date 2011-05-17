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

void ClProAsServer::HandleBind(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS("Bound TCP socket: " << server->GetLocal());
}

void ClProAsServer::HandleRelease(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS("Released TCP socket: " << server->GetLocal());
}

void ClProAsServer::HandleListen(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS(
			"Listening on TCP socket: " << server->GetLocal());
}

void ClProAsServer::HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS("Accepted TCP endpoint: " << address);
}

void ClProAsServer::HandleDropEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS("Dropped TCP endpoint: " << address);
}

void ClProAsServer::HandleRecvEndpoint(CcSocket* caller, CcAddress address) { 
	CcServerMulti* server = (CcServerMulti*)caller;
	
	std::string message;
	if(server->datastreams[address]->Extract(&message, 
				ClProLang::Hdr, ClProLang::Trl,
				CcStreamer::Forward) == false)
		return;

	int pid;
	std::string function, path0, path1;
	std::string pipename, addressD, addressC;
	
	if(this->language.IsFork(message.c_str())) {
		pid = this->Fork();
		if(pid > 0) {
			server->Send(language.Ok(pid), address);
			CcLogInfoS("Fork from " << address << ": " 
					<< pid);
		} else {
			server->Send(language.Error(ClProLang::ForkFailed), address);
			CcLogWarningS("Fork from " << address << ": " 
					<< pid << " ForkFailed");
		}
	} else if(this->language.IsLaunch(message.c_str(), &pid, &function)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS("Launch from " << address << ": " 
					<< pid << ", " << function << " NotFound");
		} else {
			function.append("\n");
			process->Launch(function);
			server->Send(language.Ok(pid), address);
			CcLogInfoS("Launch from " << address << ": " 
					<< pid << " " << function);
		}
	} else if(this->language.IsTerminate(message.c_str(), &pid)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS("Terminate from " << address << ": " 
					<< pid << " NotFound");
		} else {
			process->Terminate();
			this->Remove(pid);
			server->Send(language.Ok(pid), address);
			CcLogInfoS("Terminate from " << address << ": " 
					<< pid);
		}
	} else if(this->language.IsIsAlive(message.c_str(), &pid)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
				CcLogWarningS("IsAlive from " << address << ": " 
						<< pid << " NotFound");
		} else {
			if(process->IsAlive()) {
				server->Send(language.Ok(pid), address);
				//CcLogDebug(this->_stream, "IsAlive from " << address << ": " 
				//		<< pid << " Ok");
			} else {
				server->Send(language.Error(ClProLang::IsDead), address);
				char buffer[CCCORE_1MB];
				process->Read(buffer, CCCORE_1MB);
				CcLogWarningS("IsAlive from " << address << ": " 
						<< pid << " IsDead, " << buffer);
			}
		}
	} else if(this->language.IsChangeDirectory(message.c_str(), &pid, &path0)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS("ChangeDirectory from " << address << ": " 
					<< pid << ", " << path0 << " NotFound");
		} else {
			process->ChangeDirectory(path0);
			server->Send(language.Ok(pid), address);
			CcLogInfoS("ChangeDirectory from " << address << ": " 
					<< pid << " " << path0);
		}
	} else if(this->language.IsInclude(message.c_str(), &pid, &path0, &path1)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS("Include from " << address << ": " 
					<< pid  << path0 << ", " << path1 << " NotFound");
		} else {
			process->Include(path0, path1);
			server->Send(language.Ok(pid), address);
			CcLogInfoS("Include from " << address << ": " 
					<< pid << " " << path0 << ", " << path1);
		}
	} else if(this->language.IsLaunchNDF(message.c_str(), &pid, &function,
				&pipename, &addressD, &addressC)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogInfoS("LaunchNDF from " << address << ": " 
					" PID=" << pid << 
					" M-Fun=" << function << 
					", NDF=" << pipename << 
					", TiD=" << addressD << 
					", TiC=" << addressC << " NotFound");
		} else {
			process->LaunchNDF(function, pipename, addressD, addressC);
			server->Send(language.Ok(pid), address);
			CcLogInfoS("LaunchNDF from " << address << ": " 
					" PID=" << pid << 
					", M-Fun=" << function << 
					", NDF=" << pipename << 
					", TiD=" << addressD << 
					", TiC=" << addressC);
		}
	} else {
		CcLogWarningS("Message from " << address << 
				" not understood: " << message); 
		server->Send(language.Error(ClProLang::NotUndestood), address);
	}
}

void ClProAsServer::Register(CcServerMulti* server) {
	CB_CcSocket(server->iOnBind, this, HandleBind);
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnListen, this, HandleListen);
	CB_CcSocket(server->iOnAcceptEndpoint, this, HandleAcceptEndpoint);
	CB_CcSocket(server->iOnDropEndpoint, this, HandleDropEndpoint);
	CB_CcSocket(server->iOnRecvEndpoint, this, HandleRecvEndpoint);
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
