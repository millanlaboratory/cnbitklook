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

#ifndef CCCLIENT_HPP
#define CCCLIENT_HPP

#include "CcCallback.hpp"
#include "CcBasic.hpp"
#include "CcSocket.hpp"
#include "CcEndpoint.hpp"
#include "CcThread.hpp"
#include "CcStreamer.hpp"
#include <string>

/*! \brief Asynchronous TCP client
 *
 * It relies  on CcSocket and more deeply on libtransport.
 */
class CcClient : public CcSocket, public CcThread {
	public: 
		CcClient(size_t bsize = 128*CCCORE_1KB, unsigned int maxconns = 1);
		virtual ~CcClient(void);
		void Connect(const CcEndpoint endpoint, const unsigned int wait = 0);
		void Connect(const CcAddress address, const unsigned int wait = 0);
		void Connect(const CcIp ip, const CcPortUInt port, 
				const unsigned int wait = 0);
		void Disconnect(void);
		int Send(const char* message);
		int Send(std::string* message);
		bool SendRecv(const char* query, std::string *reply, 
				std::string hdr, std::string trl, float waitms = -1);
		bool SendRecv(const std::string& query, std::string *reply, 
				std::string hdr, std::string trl, float waitms = -1);
		CcAddress GetRemote(void);
		CcAddress GetLocal(void);
	protected:
		int Recv(void);
		void Main(void);
		void OpenSocket(void);
		void CloseSocket(void);
	private:
		
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
