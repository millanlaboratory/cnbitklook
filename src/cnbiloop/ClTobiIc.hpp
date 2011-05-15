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

#ifndef CLTOBIIC_HPP 
#define CLTOBIIC_HPP 

#include "ClLoop.hpp"
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcServer.hpp>
#include <cnbicore/CcSocketProxy.hpp>
#include <tobicore/TCException.hpp>
#include <tobiic/ICMessage.hpp>
#include <tobiic/ICSerializerRapid.hpp>

class ClTobiIc : public CcSocketProxy {
	public:
		ClTobiIc(void);
		virtual ~ClTobiIc(void);
		bool Attach(const CcPort port, const std::string& name);
		bool Detach(void);
		bool IsAttached(void);
		int WaitMessage(ICSerializerRapid* serializer);
		int GetMessage(ICSerializerRapid* serializer);
	protected:
		int Deserialize(ICSerializerRapid* serializer);
		void HandleAccept(CcSocket* caller);
		void HandleDrop(CcSocket* caller);
		void HandleRecv(CcSocket* caller);

	public:
		const static int Detached = -1;
		const static int HasMessage = 1;
		const static int NoMessage = 0;
	protected:
		CcServer* _server;
		std::string _name;
		std::string _buffer;
		CcSemaphore _sembuffer;
		CcSemaphore _hasmessage;
};

#endif
