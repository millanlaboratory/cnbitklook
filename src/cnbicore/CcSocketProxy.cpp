/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    his program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY { } without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCSOCKETPROXY_CPP 
#define CCSOCKETPROXY_CPP 

#include "CcSocketProxy.hpp" 
#include "CcServer.hpp" 
#include <cnbicore/CcLog.hpp>

void CcSocketProxy::HandleBind(CcSocket* caller) {
	CcLogDebugS("Socket " << caller->GetFID() << " bound");
}

void CcSocketProxy::HandleRelease(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " released");
}

void CcSocketProxy::HandleListen(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " listening");
}

void CcSocketProxy::HandleAccept(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " accepted");
}

void CcSocketProxy::HandleDrop(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " dropped");
}

void CcSocketProxy::HandleAcceptEndpoint(CcSocket* caller, CcAddress addr) { 
	CcLogDebugS("Socket " << caller->GetFID() << " accepted " << addr);
}

void CcSocketProxy::HandleDropEndpoint(CcSocket* caller, CcAddress addr) { 
	CcLogDebugS("Socket " << caller->GetFID() << " dropped " << addr);
}

void CcSocketProxy::HandleSend(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " sent data");
}

void CcSocketProxy::HandleRecv(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " received data");
}

void CcSocketProxy::HandleSendEndpoint(CcSocket* caller, CcAddress addr) { 
	CcLogDebugS("Socket " << caller->GetFID() << " sent to " << addr);
}

void CcSocketProxy::HandleRecvEndpoint(CcSocket* caller, CcAddress addr) { 
	CcLogDebugS("Socket " << caller->GetFID() << " received from " << addr);
}

void CcSocketProxy::HandleAccept(CcSocket* caller, CcSocket* ep) { 
	CcLogDebugS("Socket " << caller->GetFID() << " accepted " << ep->GetFID());
}

void CcSocketProxy::HandleDrop(CcSocket* caller, CcSocket* ep) { 
	CcLogDebugS("Socket " << caller->GetFID() << " dropped " << ep->GetFID());
}

void CcSocketProxy::HandleSend(CcSocket* caller, CcSocket* ep, const char* msg) {
	CcLogDebugS("Socket " << caller->GetFID() << " sent to " << ep->GetFID() <<
			": " << msg);
}

void CcSocketProxy::HandleRecv(CcSocket* caller, CcSocket* ep, const char* msg) { 
	CcLogDebugS("Socket " << caller->GetFID() << " received from " <<
			ep->GetFID() << ": " << msg);
}

void CcSocketProxy::HandleConnect(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " connected");
}

void CcSocketProxy::HandleDisconnect(CcSocket* caller) { 
	CcLogDebugS("Socket " << caller->GetFID() << " disconnected");
}

#endif
