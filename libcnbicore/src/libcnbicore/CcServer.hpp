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

#ifndef CCSERVER_HPP
#define CCSERVER_HPP

#include "CcEndpoint.hpp"
#include "CcNetworkTypes.hpp"
#include "CcSocket.hpp"
#include "CcThread.hpp"
#include "CcThreadSafe.hpp"
#include "CcBasic.hpp"
#include <string>

typedef int CcServerRole;
/*! \brief TCP server basic functionalities
 *
 * This class has the look and feel of a network server.
 */
class CcServer : public CcSocket, public CcThread {
	public:
		CcServer(CcServerRole role = CcServer::AsServer, 
				unsigned int bsize = CCCORE_1MB, 
				unsigned int maxconns = 256);
		~CcServer(void);
		virtual int Recv(void) = 0; 
		virtual int Send(const char* message) = 0;
		virtual int Send(std::string* message) = 0;
		virtual void Bind(const CcEndpoint enpoint, const unsigned int wait = 0);
		virtual void Bind(const CcAddress address, const unsigned int wait = 0);
		virtual void Bind(const CcPortUInt port, const unsigned int wait = 0);
		virtual void Bind(const tr_socket* const master);
		virtual void Release(void);
		virtual CcAddress GetLocal(void) ;
	protected:
		virtual void Listen(void);
		virtual void OpenSocket(void);
		virtual void CloseSocket(void);

	public: 
		const static CcServerRole AsServer = 0;
		const static CcServerRole AsEndpoint = 1;
	protected:
		CcEndpoint _hostLocal;
		CcThreadSafe<CcServerRole> _role;
	public:
		CcCallback1<CcSocketProxy, CcSocket*> iOnBind;
		CcCallback1<CcSocketProxy, CcSocket*> iOnRelease;
		CcCallback1<CcSocketProxy, CcSocket*> iOnListen;
	protected:
		virtual void pOnBind(void) {};
		virtual void pOnRelease(void) {};
		virtual void pOnListen(void) {};
};

#endif
