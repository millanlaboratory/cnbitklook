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

#ifndef CCSERVERSINGLE_HPP
#define CCSERVERSINGLE_HPP

#include "CcCallback.hpp"
#include "CcEndpoint.hpp"
#include "CcSemaphore.hpp"
#include "CcServer.hpp"
#include "CcSocket.hpp"
#include <string>

/*! \brief TCP server that handles a single endpoint
 */
class CcServerSingle : public CcServer {
	public:
		CcServerSingle(size_t bsize = CCCORE_1MB, unsigned maxconns = 1);
		virtual ~CcServerSingle(void);
		int Send(const char* message);
		int Send(std::string* message);
		bool SendRecv(std::string *query, std::string *reply, 
				std::string hdr, std::string trl, float waitms = -1);
		CcAddress GetRemote(void);
		void Drop(void);
		bool IsConnected(void);
	protected:
		int Recv(void); 
		void Main(void);
		bool Accept(void);

	private:
		tr_socket _endpoint;
		CcSemaphore _semendpoint;
		CcEndpoint _hostRemote;

	public:
		CcCallback1<CcSocketProxy, CcSocket*> iOnAccept;
		CcCallback1<CcSocketProxy, CcSocket*> iOnDrop;
	protected:
		virtual void pOnAccept(void) {};
		virtual void pOnDrop(void) {};
};

#endif
