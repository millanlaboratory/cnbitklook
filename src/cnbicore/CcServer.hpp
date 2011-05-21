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

#ifndef CCSERVER_HPP 
#define CCSERVER_HPP 

#include <cnbicore/CcSocket.hpp>
#include <cnbicore/CcThread.hpp>

/*! \brief TCP/UDP multiplexing server
 *
 * CcServer is a multiplexing TCP/UDP server.
 */
class CcServer : public CcSocket, protected CcThread {
	public:
		CcServer(size_t bsize = CCCORE_1MB);
		virtual ~CcServer(void);
		bool Bind(CcPort port, int protocol = CcSocket::TCP);
		bool Bind(CcPortUInt port, int protocol = CcSocket::TCP);
		bool Release(void);
		bool IsConnected(void);
		
		ssize_t Send(const char* message);
		ssize_t Send(const std::string& message);
		ssize_t Send(const void* message, size_t size);
		
		ssize_t Send(const char* message, CcAddress addr);
		ssize_t Send(const std::string& message, CcAddress addr);
		ssize_t Send(const void* message, size_t size, CcAddress addr);
		
		ssize_t SendNot(const char* message, CcAddress addr);
		ssize_t SendNot(const std::string& message, CcAddress addr);
		ssize_t SendNot(const void* message, size_t size, CcAddress addr);
	private:
		void Main(void);
		bool Open(int protocol);
		bool Listen(void);
		int Accept(void);
		void Drop(int fid);

	public:
		CcCallback1<CcSocketProxy, CcSocket*> iOnRelease;
		CcCallback1<CcSocketProxy, CcSocket*> iOnAccept;
		CcCallback1<CcSocketProxy, CcSocket*> iOnDrop;
		CcCallback2<CcSocketProxy, CcSocket*, CcAddress> iOnAcceptPeer;
		CcCallback2<CcSocketProxy, CcSocket*, CcAddress> iOnDropPeer;
		CcCallback3<CcSocketProxy, CcSocket*, CcAddress, CcStreamer*> iOnRecvPeer;
	protected:
		CcCallback1<CcSocketProxy, CcSocket*> iOnBind;
		CcCallback1<CcSocketProxy, CcSocket*> iOnListen;
		CcCallback2<CcSocketProxy, CcSocket*, CcAddress> iOnSendPeer;
};

#endif
