/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    his program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCSOCKETPROXY_HPP 
#define CCSOCKETPROXY_HPP 

#include "CcSocket.hpp"

class CcSocketProxy {
	public:
		virtual void HandleBind(CcSocket* caller);
		virtual void HandleRelease(CcSocket* caller);
		virtual void HandleListen(CcSocket* caller);
		
		virtual void HandleAccept(CcSocket* caller);
		virtual void HandleDrop(CcSocket* caller);
		
		virtual void HandleAcceptPeer(CcSocket* caller, CcAddress addr);
		virtual void HandleDropPeer(CcSocket* caller, CcAddress addr);
		
		virtual void HandleSend(CcSocket* caller);
		virtual void HandleRecv(CcSocket* caller, CcStreamer* stream);
		
		virtual void HandleSendPeer(CcSocket* caller, CcAddress addr);
		virtual void HandleRecvPeer(CcSocket* caller, CcAddress addr, CcStreamer* stream);

		virtual void HandleAccept(CcSocket* caller, CcSocket* ep);
		virtual void HandleDrop(CcSocket* caller, CcSocket* ep);
		
		virtual void HandleConnect(CcSocket* caller);
		virtual void HandleDisconnect(CcSocket* caller);
};

#define CB_CcSocket(sender_event, receiver_ptr, method) 		\
	sender_event.Register((CcSocketProxy*)receiver_ptr, 		\
			&CcSocketProxy::method);

#endif
