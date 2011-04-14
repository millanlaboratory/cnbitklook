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
#include <libcnbicore/CcCore.hpp>
#include <libcnbicore/CcObject.hpp>
#include <libcnbicore/CcSocket.hpp>
#include <libcnbicore/CcServerSingle.hpp>
#include <libcnbicore/CcTime.hpp>
#include <stdio.h>

class EventHandler : public CcSocketProxy {
	public:
		void HandleBind(CcSocket* caller) { 
			CcServerSingle *server = (CcServerSingle*)caller;
			printf("[EventHandler] Server acquired local TCP socket: %s\n", 
					server->GetLocal().c_str());
		}
	
		void HandleRelease(CcSocket* caller) { 
			CcServerSingle *server = (CcServerSingle*)caller;
			printf("[EventHandler] Server released local TCP socket: %s\n", 
					server->GetLocal().c_str());
		}
		
		void HandleListen(CcSocket* caller) { 
			//CcServerSingle *server = (CcServerSingle*)caller;
			printf("[EventHandler] Server is listening\n");
		}
		
		void HandleAccept(CcSocket* caller) { 
			CcServerSingle *server = (CcServerSingle*)caller;
			printf("[EventHandler] TCP Enpoint accepted: %s\n",
					server->GetRemote().c_str());
		}
		
		void HandleDrop(CcSocket* caller) { 
			CcServerSingle *server = (CcServerSingle*)caller;
			printf("[EventHandler] TCP Enpoint dropped the connection: %s\n", 
					server->GetRemote().c_str());
		}
		
		void HandleSend(CcSocket* caller) { 
			//CcServerSingle *server = (CcServerSingle*)caller;
			printf("[EventHandler] Server sent a message\n");
		}
		
		void HandleRecv(CcSocket* caller) { 
			CcServerSingle* server = (CcServerSingle*)caller;
			printf("[EventHandler] Server received a message\n");
			server->Send((const char*)"Message received");
			server->datastream.Clear();
			printf("[EventHandler] Streamer now empty: '%d'\n", 
					server->datastream.Size());
		}
};

void RegisterAll(CcServerSingle* server, EventHandler *handler) {
	CB_CcSocket(server->iOnBind,   handler, HandleBind);
	CB_CcSocket(server->iOnRelease, handler, HandleRelease);
	CB_CcSocket(server->iOnListen, handler, HandleListen);
	CB_CcSocket(server->iOnAccept, handler, HandleAccept);
	CB_CcSocket(server->iOnDrop,   handler, HandleDrop);
	CB_CcSocket(server->iOnRecv,   handler, HandleRecv);
	CB_CcSocket(server->iOnSend,   handler, HandleSend);
}

int main(void) {
	EventHandler *handler = new EventHandler();
	
	CcServerSingle *server;
	
	printf("[Main] Starting server\n");
	server = new CcServerSingle();
	RegisterAll(server, handler);
	server->Bind(8000, 2);
	//server->Listen();
	//server->Start();

	printf("[Main] Up and running\n");
	CcTime::Sleep(5000);
	std::string reply;
	std::string query("<hellothere>");
	printf("[Main] Waiting for <reply>\n");
	bool status = server->SendRecv(&query, &reply,
			std::string("<"), std::string(">"));
	printf("[Main] Reply was %d\n", status);
	//server->Join();
	CcTime::Sleep(2000000);
	printf("[Main] Stopping ASync server\n");
	//server->Stop();
	//server->Join();
	server->Release();
	
	printf("[Main] Debug: %d\n", server->IsRunning());
	CcCore::Status();
	delete server;
		
	CcCore::Status();
	return 0;
}
