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

#include <libcnbicore/CcClient.hpp>
#include <libcnbicore/CcCore.hpp>
#include <libcnbicore/CcTime.hpp>
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
		
		void HandleSend(CcSocket* caller) { 
			printf("[EventHandler] Client sent a message\n");
		}
		
		void HandleRecv(CcSocket* caller) { 
			printf("[EventHandler] Client received a message\n");
		}
};

void RegisterAll(CcClient* client, EventHandler *handler) {
	CB_CcSocket(client->iOnConnect,    handler, HandleConnect);
	CB_CcSocket(client->iOnDisconnect, handler, HandleDisconnect);
	CB_CcSocket(client->iOnRecv,   	   handler, HandleRecv);
	CB_CcSocket(client->iOnSend,       handler, HandleSend);
}

int main(void) {
	CcCore::OpenLogger("ccnsclient", CcCore::TerminalColors);
	
	CcClient* client = new CcClient();
	EventHandler* handler = new EventHandler();

	RegisterAll(client, handler);
	
	while(true) {
		try {
			client->Connect("127.0.0.1", 8000, 5);
		} catch(CcException e) {
			printf("Exception: %s: %s\n", e.Caller().c_str(), e.Info().c_str());
			CcTime::Sleep(1000);
			continue;
		}

		for(int i = 0; i < 10; i++) {
			client->Send((const char*)"Hello there!");
			CcTime::Sleep(1000);
		}
		client->Disconnect();
		CcTime::Sleep(2500);
	}
	
	client->Join();
	client->Disconnect();
	delete client;

	CcCore::Status();
	return 0;
}
