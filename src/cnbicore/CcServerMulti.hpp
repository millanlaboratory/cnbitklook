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
		CcServerMulti(bool multistream = true, 
				double acceptms = CCASYNC_WAIT_MACCEPT, 
				double recvms = CCASYNC_WAIT_MRECV, 
				size_t bsize = 128*CCCORE_1KB, 
				unsigned maxconns = 256);
		virtual ~CcServerMulti(void);
		
		int Send(const char* message);
		int Send(const char* message, CcAddress address);
		int Send(std::string* message);
		int Send(std::string* message, CcAddress address);
		int SendNot(const char* message, CcAddress address);
		int SendNot(std::string* message, CcAddress address);
		bool SendRecv(std::string *query, std::string *reply, 
				std::string hdr, std::string trl, float waitms = -1);
		void Drop(void);
		void DumpEndpoints(void);
		CcEndpointList GetRemote(void);
	protected:
		int Recv(void); 
		void Main(void);
		bool Accept(CcAddress* raddress = NULL);
	private:
		int ImplRecv(CcAddress address); 
		int ImplSend(const char* message, CcAddress address);
		int ImplDrop(CcAddress address);
		bool ImplHasEndpoint(CcAddress address);
		bool ImplHasHost(CcAddress address);
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
		double _acceptms;
		double _recvms;
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
