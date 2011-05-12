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

#ifndef CCSOCKET_HPP
#define CCSOCKET_HPP

#include "CcFlags.hpp"
#include "CcCallback.hpp"
#include "CcMutex.hpp"
#include "CcObject.hpp"
#include "CcSemaphore.hpp"
#include "CcStreamer.hpp"
#include "CcThreadSafe.hpp"
#include <libtransport/tr_net.h>

/* Forward declaration */
class CcSocketProxy;

/*! \brief TCP socket
 * 
 * Implements the basic socket functionalities.
 */
class CcSocket : public CcObject {
	public:
		CcSocket(size_t bsize = CCCORE_1MB, unsigned int maxconn = 1);
		virtual ~CcSocket(void);
		virtual int Recv(void) = 0; 
		virtual int Send(const char* message) = 0;
		virtual int Send(std::string* message) = 0;
		long unsigned int GetBytesSend(void);
		long unsigned int GetBytesRecv(void);
		virtual bool IsConnected(void);
		size_t GetBsize(void);
	protected:
		virtual void OpenSocket(void) = 0;
		virtual void CloseSocket(void) = 0;
		bool SetNonBlocking(bool value);
		void AddBytesSend(const int bytes);	
		void AddBytesRecv(const int bytes);
	private:
		virtual void AllocBuffer(size_t bsize, unsigned int maxconn);
		virtual void FreeBuffer(void);

	public:
		CcStreamer datastream;
	protected:
		tr_socket* _socket;
		CcSemaphore _semsocket;
		char* _buffer;
		size_t _bsize;
		unsigned int _maxconn;
		CcSemaphore _sembuffer;
	private:
		CcMutex _mtxbytes;
		long unsigned int _bytesRecv;
		long unsigned int _bytesSend;
	public:
		const static CcHostname HostnameUnset;
		const static CcPort PortUnset;
		const static CcPortUInt PortUintUnset = 0;
		const static CcIp IpUnset;
		const static CcAddress AddressUnset;
		
	public:
		CcCallback1<CcSocketProxy, CcSocket*> iOnSend;
		CcCallback1<CcSocketProxy, CcSocket*> iOnRecv;
	protected:
		virtual void pOnRecv(void) {};
		virtual void pOnSend(void) {};
};


/*! \brief Callback proxy for CcSocket
 */
class CcSocketProxy {
	public:
		virtual void HandleBind(CcSocket* caller) { }
		virtual void HandleRelease(CcSocket* caller) { }
		virtual void HandleListen(CcSocket* caller) { }
		
		virtual void HandleAccept(CcSocket* caller) { }
		virtual void HandleDrop(CcSocket* caller) { }
		
		virtual void HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { }
		virtual void HandleDropEndpoint(CcSocket* caller, CcAddress address) { }
		
		virtual void HandleSend(CcSocket* caller) { }
		virtual void HandleRecv(CcSocket* caller) { }
		
		virtual void HandleSendEndpoint(CcSocket* caller, CcAddress address) { }
		virtual void HandleRecvEndpoint(CcSocket* caller, CcAddress address) { }

		virtual void HandleAccept(CcSocket* caller, CcSocket* endpoint) { }
		virtual void HandleDrop(CcSocket* caller, CcSocket* endpoint) { }
		
		virtual void HandleSend(CcSocket* caller, CcSocket* endpoint,
				const char* message) { }
		virtual void HandleRecv(CcSocket* caller, CcSocket* endpoint, 
				const char* message) { }
		
		virtual void HandleConnect(CcSocket* caller) { }
		virtual void HandleDisconnect(CcSocket* caller) { }
};

#define CB_CcSocket(sender_event, receiver_ptr, method) 		\
	sender_event.Register((CcSocketProxy*)receiver_ptr, 		\
			&CcSocketProxy::method);
#endif
