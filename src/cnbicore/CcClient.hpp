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

#include "CcSocket.hpp"
#include "CcThread.hpp"

class CcClient : public CcSocket, protected CcThread {
	public:
		CcClient(size_t bsize = CCCORE_1MB);
		virtual ~CcClient(void);
		bool Connect(CcAddress address, int protocol = CcSocket::TCP);
		bool Disconnect(void);
		ssize_t Send(const char* message);
		ssize_t Send(const std::string& message);
		ssize_t Send(const void* message, size_t size);

	private:
		void Main(void);
		bool Open(int protocol);

	public:
		CcCallback1<CcSocketProxy, CcSocket*> iOnConnect;
		CcCallback1<CcSocketProxy, CcSocket*> iOnDisconnect;
};



#endif
