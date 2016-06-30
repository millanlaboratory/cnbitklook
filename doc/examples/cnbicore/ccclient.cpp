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

#include <cnbicore/CcClient.hpp>
#include <cnbicore/CcCore.hpp>
#include <cnbicore/CcEndpoint.hpp>
#include <cnbicore/CcSocketProxy.hpp>
#include <cnbicore/CcTime.hpp>
#include <stdio.h>

class EventHandler : public CcSocketProxy {
	public:
		void HandleConnect(CcSocket* caller) { 
			CcClient *client = (CcClient*)caller;
			printf("[EventHandler] Client acquired local TCP socket: %s\n", 
					client->GetLocal().c_str());
			printf("[EventHandler] Client connected remote TCP socket: %s\n", 
					client->GetRemote().c_str());
		}
	
		void HandleDisconnect(CcSocket* caller) { 
			CcClient *client = (CcClient*)caller;
			printf("[EventHandler] Client disconnected & local TCP socket released: %s\n", 
					client->GetLocal().c_str());
		}
		
		void HandleRecv(CcSocket* caller) { 
			printf("[EventHandler] Client received a message\n");
		}
};

void RegisterAll(CcClient* client, EventHandler *handler) {
	CB_CcSocket(client->iOnConnect,    handler, HandleConnect);
	CB_CcSocket(client->iOnDisconnect, handler, HandleDisconnect);
	CB_CcSocket(client->iOnRecv,   	   handler, HandleRecv);
}

int main(void) {
	CcCore::OpenLogger("ccnsclient", CcCore::TerminalColors);
	
	CcClient* client = new CcClient();
	EventHandler* handler = new EventHandler();

	RegisterAll(client, handler);
	CcEndpoint peer("127.0.0.1", "8000");
	while(true) {
		while(client->Connect(peer.GetAddress()) == false) {
			printf("Waiting for connection...\n");
			CcTime::Sleep(1000);
		}

		for(int i = 0; i < 20; i++) {
			client->Send((const char*)"Hello there!");
			if(client->IsConnected() == false) {
				printf("Disconnected...\n");
				break;
			}
			CcTime::Sleep(1000);
		}
		client->Disconnect();
		CcTime::Sleep(2500);
	}
	
	client->Disconnect();
	delete client;

	CcCore::Status();
	return 0;
}
