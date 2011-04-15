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

#include "ClNamesClient.hpp"
#include <libcnbicore/CcClient.hpp>
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcServerMulti.hpp>
#include <libtobiid/IDMessage.hpp>
#include <libtobiid/IDSerializerRapid.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


using namespace std;

class TiDHandler : public CcSocketProxy {
	public:
		TiDHandler(void) {
			this->_idserializer = new IDSerializerRapid(&this->_idmessage);
		}
		
		void HandleConnect(CcSocket* caller) { 
			CcClient *client = (CcClient*)caller;
			CcLogConfigS(this->_stream, "Client " << client->GetLocal() <<
					" connected to " << client->GetRemote());
		}

		void HandleDisconnect(CcSocket* caller) { 
			CcClient *client = (CcClient*)caller;
			CcLogConfigS(this->_stream, "Client " << client->GetLocal() <<
					" disconnected from " << client->GetRemote());
		}

		void HandleRecv(CcSocket* caller) { 
			std::string message, iddump, absolute, relative;
			bool status = false;
			
			while(true) {
				status = caller->datastream.Extract(&message, 
						"<tobiid", "/>", CcStreamer::Forward);
				if(status == false)
					break;
				this->_idserializer->Deserialize(&message);
				this->_idmessage.absolute.Get(&absolute);
				this->_idmessage.relative.Get(&relative);

				CcLogInfoS(this->_stream, "TiD event:" << 
				" A/R=" << absolute << 
				"/" << relative << 
				", Family=" << this->_idmessage.GetFamily() << 
				", Event=" << this->_idmessage.GetEvent() << 
				", Description=" << this->_idmessage.GetDescription());

			}
		}
	private:
		IDMessage _idmessage;
		IDSerializerRapid* _idserializer;
		std::stringstream _stream;
};

void usage(void) { 
	printf("Usage: cl_tidmonitor [-a IP:PORT]\n");
	printf("Where: -a       127.0.0.1:9000 (default)\n");
	exit(1);
}

int main(int argc, char* argv[]) {
	// Parse command line
	int opt;
	std::string optopt;
	CcEndpoint optendpoint("127.0.0.1:9000");
	
	while ((opt = getopt(argc, argv, "a:h")) != -1) {
		if(opt == 'a')
			optendpoint.SetAddress(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}
	
	CcCore::OpenLogger("cl_tidmonitor");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	
	CcLogInfo(std::string("TiD monitor configured: ").
			append(optendpoint.GetAddress()).append("/TCP"));
	
	// Setup TCP client 
	CcClient client;
	TiDHandler handler;
	CB_CcSocket((&client)->iOnRecv, &handler, HandleRecv);
	CB_CcSocket((&client)->iOnConnect, &handler, HandleConnect);
	CB_CcSocket((&client)->iOnDisconnect, &handler, HandleDisconnect);

	ClNamesClient nsclient;
	try {
		client.Connect(optendpoint);
	} catch(CcException e) {
		CcLogFatal("Cannot connect to acquisition");
		exit(1);
	}

	if(nsclient.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		exit(2);
	}
	int nsstatus = nsclient.Set("/tidmonitor", client.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		exit(3);
	}
	while(true) {
		if(nsclient.Connect() == false)  {
			CcLogFatal("Lost connection with nameserver");
			//client.Disconnect();
			break;
		}
		if(client.IsConnected() == false) {
			nsclient.Unset("/tidmonitor");
			CcLogFatal("Lost connection with acquisition");
			break;
		}
		if(CcCore::receivedSIGINT.Get()) {
			//client.Disconnect();
			nsclient.Unset("/tidmonitor");
			break;
		}
		if(CcCore::receivedSIGTERM.Get()) {
			//client.Disconnect();
			nsclient.Unset("/tidmonitor");
			break;
		}
		CcTime::Sleep(2000);
	}
	//nsclient.Unset("/tidmonitor");

	return 0;
}
