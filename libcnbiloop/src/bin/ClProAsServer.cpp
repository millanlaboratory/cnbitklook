#ifndef CLPROASSERVER_CPP 
#define CLPROASSERVER_CPP 

#include "ClProAsServer.hpp" 

void ClProAsServer::HandleBind(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS(this->_stream, "Bound TCP socket: " << server->GetLocal());
}

void ClProAsServer::HandleRelease(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS(this->_stream, "Released TCP socket: " << server->GetLocal());
}

void ClProAsServer::HandleListen(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS(this->_stream, 
			"Listening on TCP socket: " << server->GetLocal());
}

void ClProAsServer::HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS(this->_stream, "Accepted TCP endpoint: " << address);
}

void ClProAsServer::HandleDropEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS(this->_stream, "Dropped TCP endpoint: " << address);
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
	std::string pipename, addressD, addressC, extra;
	
	if(this->language.IsFork(message.c_str())) {
		pid = this->Fork();
		if(pid > 0) {
			server->Send(language.Ok(pid), address);
			CcLogInfoS(this->_stream, "Fork from " << address << ": " 
					<< pid);
		} else {
			server->Send(language.Error(ClProLang::ForkFailed), address);
			CcLogWarningS(this->_stream, "Fork from " << address << ": " 
					<< pid << " ForkFailed");
		}
	} else if(this->language.IsLaunch(message.c_str(), &pid, &function)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS(this->_stream, "Launch from " << address << ": " 
					<< pid << ", " << function << " NotFound");
		} else {
			function.append("\n");
			process->Launch(function);
			server->Send(language.Ok(pid), address);
			CcLogInfoS(this->_stream, "Launch from " << address << ": " 
					<< pid << " " << function);
		}
	} else if(this->language.IsTerminate(message.c_str(), &pid)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS(this->_stream, "Terminate from " << address << ": " 
					<< pid << " NotFound");
		} else {
			process->Terminate();
			this->Remove(pid);
			server->Send(language.Ok(pid), address);
			CcLogInfoS(this->_stream, "Terminate from " << address << ": " 
					<< pid);
		}
	} else if(this->language.IsIsAlive(message.c_str(), &pid)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
				CcLogWarningS(this->_stream, "IsAlive from " << address << ": " 
						<< pid << " NotFound");
		} else {
			if(process->IsAlive()) {
				server->Send(language.Ok(pid), address);
				//CcLogDebug(this->_stream, "IsAlive from " << address << ": " 
				//		<< pid << " Ok");
			} else {
				server->Send(language.Error(ClProLang::IsDead), address);
				char buffer[20480];
				process->Read(buffer, 20480);
				CcLogWarningS(this->_stream, "IsAlive from " << address << ": " 
						<< pid << " IsDead, " << buffer);
			}
		}
	} else if(this->language.IsChangeDirectory(message.c_str(), &pid, &path0)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS(this->_stream, "ChangeDirectory from " << address << ": " 
					<< pid << ", " << path0 << " NotFound");
		} else {
			process->ChangeDirectory(path0);
			server->Send(language.Ok(pid), address);
			CcLogInfoS(this->_stream, "ChangeDirectory from " << address << ": " 
					<< pid << " " << path0);
		}
	} else if(this->language.IsInclude(message.c_str(), &pid, &path0, &path1)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogWarningS(this->_stream, "Include from " << address << ": " 
					<< pid  << path0 << ", " << path1 << " NotFound");
		} else {
			process->Include(path0, path1);
			server->Send(language.Ok(pid), address);
			CcLogInfoS(this->_stream, "Include from " << address << ": " 
					<< pid << " " << path0 << ", " << path1);
		}
	} else if(this->language.IsLaunchNDF(message.c_str(), &pid, &function,
				&pipename, &addressD, &addressC, &extra)) {
		ClMatlab* process = this->Get(pid);
		if(process == NULL) {
			server->Send(language.Error(ClProLang::NotFound), address);
			CcLogInfoS(this->_stream, "LaunchNDF from " << address << ": " 
					<< pid << 
					" M-Fun=" << function << 
					", NDF=" << pipename << 
					", TiD=" << addressD << 
					", TiC=" << addressC << 
					", Args=" << extra << "NotFound");
		} else {
			process->LaunchNDF(function, pipename, addressD, addressC, extra);
			server->Send(language.Ok(pid), address);
			CcLogInfoS(this->_stream, "LaunchNDF from " << address << ": " 
					<< pid << 
					" M-Fun=" << function << 
					", NDF=" << pipename << 
					", TiD=" << addressD << 
					", TiC=" << addressC << 
					", Args=" << extra);
		}
	} else {
		CcLogWarningS(this->_stream, "Message from " << address << 
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
