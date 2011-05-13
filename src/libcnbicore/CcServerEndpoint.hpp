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

#ifndef CCSERVERENDPOINT_HPP
#define CCSERVERENDPOINT_HPP

#include "CcCallback.hpp"
#include "CcEndpoint.hpp"
#include "CcSemaphore.hpp"
#include "CcServer.hpp"
#include "CcSocket.hpp"
#include <string>

/*! \brief TCP server that handles a single endpoint
 */
class CcServerEndpoint : public CcServer {
	public:
		CcServerEndpoint(unsigned int bsize = 1024, unsigned maxconns = 1);
		virtual ~CcServerEndpoint(void);
		virtual int Recv(void); 
		virtual int Send(const char* message);
		virtual int Send(std::string* message);
		std::string GetRemote(void);
		virtual void Drop(void);
	protected:
		virtual void Main(void);
		virtual bool Accept(void);

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
