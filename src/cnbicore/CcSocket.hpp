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

#ifndef CCSOCKET_HPP
#define CCSOCKET_HPP

#include "CcCallback.hpp"
#include "CcFlags.hpp"
#include "CcNetworkTypes.hpp"
#include "CcObject.hpp"
#include "CcSemaphore.hpp"
#include "CcStreamer.hpp"
#include "CcThreadSafe.hpp"
#include <transport/tr_net.h>
#include <map>

/* Forward declaration */
class CcSocketProxy;


/*! \brief TCP/UDP socket
 * 
 * Implements the basic socket functionalities. Objects like CcServer and
 * CcClient heavily rely on this class. It implements all the methods to send
 * and receive ASCII and binary data.
 */
class CcSocket : public CcObject {
	public:
		CcSocket(size_t bsize);
		virtual ~CcSocket(void);
		virtual CcAddress GetLocal(void);
		virtual CcAddress GetRemote(void);
		long unsigned int GetSend(void);
		long unsigned int GetRecv(void);
		unsigned int GetPeers(void);
		int GetFID(void);
		virtual bool IsConnected(void);
		void Dump(void);
		static CcAddress Lookup(std::string name);
	protected:
		virtual ssize_t Send(const char* message) = 0;
		virtual ssize_t Send(const std::string& message) = 0;
		virtual ssize_t Send(const void* message, size_t size) = 0;
		virtual ssize_t Send(tr_socket* peer, const void* message, size_t size); 
		virtual ssize_t Recv(tr_socket* peer); 
		virtual bool Open(int protocol) = 0;
		virtual bool Close(void);
		void AddSend(const unsigned int bytes);	
		void AddRecv(const unsigned int bytes);
		
		CcStreamer* GetStream(int fid);
		bool AddStream(int fid);
		bool RemStream(int fid);
		bool HasStream(int fid);
		
		tr_socket* GetPeer(int fid);
		bool AddPeer(tr_socket* peer);
		bool RemPeer(int fid);
		bool HasPeer(int fid);
		
		tr_socket* GetPeer(CcAddress addr);
		CcAddress GetAddress(int fid);

	protected:
		tr_socket* _socket;
		CcSemaphore _semsocket;
		CcStreamerMap _streams;
		CcSocketMap _peers;
	private:
		long unsigned int _bytesRecv;
		long unsigned int _bytesSend;
	public:
		const static int TCP = 0;
		const static int UDP = 1;
		CcCallback2<CcSocketProxy, CcSocket*, CcStreamer*> iOnRecv;
	protected:
		CcCallback1<CcSocketProxy, CcSocket*> iOnSend;
};

#endif
