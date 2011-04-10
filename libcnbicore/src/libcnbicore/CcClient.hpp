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

#ifndef CCCLIENT_HPP
#define CCCLIENT_HPP

#include "CcCallback.hpp"
#include "CcBasic.hpp"
#include "CcSocket.hpp"
#include "CcEndpoint.hpp"
#include "CcThread.hpp"
#include "CcStreamer.hpp"
#include <string>

/*! \brief TCP client
 */
class CcClient : public CcSocket, public CcThread {
	public: 
		CcClient(unsigned int bsize = CCSOCKET_BSIZE, unsigned int maxconns = 256);
		virtual ~CcClient(void);
		virtual void Connect(const CcEndpoint endpoint, const unsigned int wait = 0);
		virtual void Connect(const CcAddress address, const unsigned int wait = 0);
		virtual void Connect(const CcIp ip, const CcPortUInt port, 
				const unsigned int wait = 0);
		virtual void Disconnect(void);
		virtual int Recv(void);
		virtual int Send(const char* message);
		virtual int Send(std::string* message);
		virtual bool SendRecv(const char* query, std::string *reply, 
				std::string hdr, std::string trl, float waitms = -1);
		virtual bool SendRecv(const std::string& query, std::string *reply, 
				std::string hdr, std::string trl, float waitms = -1);
		CcAddress GetRemote(void);
		CcAddress GetLocal(void);
	protected:
		virtual void Main(void);
		virtual void OpenSocket(void);
		virtual void CloseSocket(void);
		
	protected:
		CcEndpoint _hostLocal;
		CcEndpoint _hostRemote;
	
	public:
		CcCallback1<CcSocketProxy, CcSocket*> iOnConnect;
		CcCallback1<CcSocketProxy, CcSocket*> iOnDisconnect;
	protected:
		virtual void pOnConnect(void) {};
		virtual void pOnDisconnect(void) {};
		virtual void pOnRecv(void) {};
		virtual void pOnSend(void) {};
};

#endif
