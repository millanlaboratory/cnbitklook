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

#ifndef CCSERVERMULTI_HPP
#define CCSERVERMULTI_HPP

#include "CcCallback.hpp"
#include "CcEndpoint.hpp"
#include "CcSemaphore.hpp"
#include "CcServer.hpp"
#include "CcSocket.hpp"
#include "CcThreadSafe.hpp"
#include <string>

/*! \brief TCP server that handles a single endpoint
 * This code needs a MAJOR review. This class is not intended for high
 * performance operations.
 */
class CcServerMulti : public CcServer {
	public:
		CcServerMulti(bool multistream = true, unsigned int bsize = CCSOCKET_BSIZE, 
				unsigned maxconns = 256);
		virtual ~CcServerMulti(void);
		
		virtual int Recv(void); 
		virtual int Send(const char* message);
		virtual int Send(const char* message, CcAddress address);
		virtual int Send(std::string* message);
		virtual int Send(std::string* message, CcAddress address);
		virtual int SendNot(const char* message, CcAddress address);
		virtual int SendNot(std::string* message, CcAddress address);
		virtual bool SendRecv(std::string *query, std::string *reply, 
				std::string hdr, std::string trl, float waitms = -1);
		virtual void Drop(void);
		virtual void DumpEndpoints(void);
		CcEndpointList GetRemote(void);
	protected:
		virtual void Main(void);
		virtual bool Accept(CcAddress* raddress = NULL);
	private:
		virtual int ImplRecv(CcAddress address); 
		virtual int ImplSend(const char* message, CcAddress address);
		virtual int ImplDrop(CcAddress address);
		virtual bool ImplHasEndpoint(CcAddress address);
		virtual bool ImplHasHost(CcAddress address);
	public:
		const static bool SingleStream = 0;
		const static bool MultipleStreams = 1;
	public:
		CcStreamerMap datastreams;
	private:
		CcSemaphore _semdatastreams;
		CcEndpointMap _hosts;
		CcSemaphore _semhosts;
		CcSocketMap _endpoints;
		CcSemaphore _semendpoints;
		tr_socket* _endpointPtr;
		CcThreadSafe<bool> _multistream;
	public:
		CcCallback1<CcSocketProxy, CcSocket*> iOnAccept;
		CcCallback1<CcSocketProxy, CcSocket*> iOnDrop;
		CcCallback2<CcSocketProxy, CcSocket*, CcAddress> iOnAcceptEndpoint;
		CcCallback2<CcSocketProxy, CcSocket*, CcAddress> iOnDropEndpoint;
		CcCallback2<CcSocketProxy, CcSocket*, CcAddress> iOnRecvEndpoint;
		CcCallback2<CcSocketProxy, CcSocket*, CcAddress> iOnSendEndpoint;
	protected:
		virtual void pOnAccept(void) {};
		virtual void pOnDrop(void) {};
};

#endif
