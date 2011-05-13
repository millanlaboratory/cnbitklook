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

#include <libcnbicore/CcCore.hpp>
#include <libcnbicore/CcObject.hpp>
#include <libcnbicore/CcSocket.hpp>
#include <libcnbicore/CcServerMulti.hpp>
#include <libcnbicore/CcTime.hpp>
#include <stdio.h>

class EventHandler : public CcSocketProxy {
	public:
		void HandleBind(CcSocket* caller) { 
			CcServerMulti *server = (CcServerMulti*)caller;
			printf("[EventHandler] Server acquired local TCP socket: %s\n", 
					server->GetLocal().c_str());
		}
	
		void HandleRelease(CcSocket* caller) { 
			CcServerMulti *server = (CcServerMulti*)caller;
			printf("[EventHandler] Server released local TCP socket: %s\n", 
					server->GetLocal().c_str());
		}
		
		void HandleListen(CcSocket* caller) { 
			//CcServerMulti *server = (CcServerMulti*)caller;
			printf("[EventHandler] Server is listening\n");
		}
		
		void HandleAccept(CcSocket* caller) { 
			printf("[EventHandler] Some TCP Enpoint accepted\n");
		}
		
		void HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { 
			printf("[EventHandler] TCP Enpoint accepted: %s\n", address.c_str());
		}
		
		void HandleDrop(CcSocket* caller) { 
			printf("[EventHandler] Some TCP Enpoint dropped the connection\n");
		}
		
		void HandleDropEndpoint(CcSocket* caller, CcAddress address) { 
			printf("[EventHandler] TCP Enpoint dropped the connection: %s\n", 
					address.c_str());
		}
		
		void HandleSend(CcSocket* caller) { 
			//CcServerMulti *server = (CcServerMulti*)caller;
			printf("[EventHandler] Server sent a message\n");
		}
		
		void HandleSendEndpoint(CcSocket* caller, CcAddress address) { 
			//CcServerMulti *server = (CcServerMulti*)caller;
			printf("[EventHandler] Server sent a message to %s\n",
					address.c_str());
		}
		
		void HandleRecv(CcSocket* caller) { 
			CcServerMulti* server = (CcServerMulti*)caller;
			printf("[EventHandler] Server received a message\n");
			server->Send((const char*)"<BounceAll>");
			server->datastream.Clear();
			printf("[EventHandler] Streamer now empty: '%d'\n", 
					server->datastream.Size());
		}
		
		void HandleRecvEndpoint(CcSocket* caller, CcAddress address) { 
			CcServerMulti* server = (CcServerMulti*)caller;
			printf("[EventHandler] Server received a message from %s\n", 
					address.c_str());
			server->SendNot("<BounceOthers>", address);
		}
};

void RegisterAll(CcServerMulti* server, EventHandler *handler) {
	CB_CcSocket(server->iOnBind,   handler, HandleBind);
	CB_CcSocket(server->iOnRelease, handler, HandleRelease);
	CB_CcSocket(server->iOnListen, handler, HandleListen);
	CB_CcSocket(server->iOnAccept, handler, HandleAccept);
	CB_CcSocket(server->iOnAcceptEndpoint, handler, HandleAcceptEndpoint);
	CB_CcSocket(server->iOnDrop,   handler, HandleDrop);
	CB_CcSocket(server->iOnDropEndpoint,   handler, HandleDropEndpoint);
	CB_CcSocket(server->iOnRecv,   handler, HandleRecv);
	CB_CcSocket(server->iOnRecvEndpoint,   handler, HandleRecvEndpoint);
	CB_CcSocket(server->iOnSend,   handler, HandleSend);
	CB_CcSocket(server->iOnSendEndpoint,   handler, HandleSendEndpoint);
}

int main(void) {
	EventHandler *handler = new EventHandler();

	CcServerMulti *server;
	
	printf("[Main] Starting server\n");
	server = new CcServerMulti();
	RegisterAll(server, handler);
	server->Bind(8000, 2);

	printf("[Main] Up and running\n");
	CcTimeValue clock;
	CcTime::Tic(&clock);
	while(CcTime::Toc(&clock) < 60000) {
		server->Send("<Multicast>");
		CcTime::Sleep(2000);
	}
	server->Drop();

	printf("[Main] Stopping ASync server\n");
	server->Release();
	
	printf("[Main] Debug: %d\n", server->IsRunning());
	delete server;
	
	return 0;
}
