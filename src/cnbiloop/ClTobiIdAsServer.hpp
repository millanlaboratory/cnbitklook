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

#ifndef CLTOBIIDASSERVER_HPP 
#define CLTOBIIDASSERVER_HPP 

#include "ClAcqLang.hpp"
#include <cnbiacq/CaWriter.hpp>
#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>
#include <tobicore/TCLanguage.hpp>
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcServer.hpp>
#include <cnbicore/CcSocketProxy.hpp>

class ClTobiIdAsServer  : public CcSocketProxy {
	public:
		ClTobiIdAsServer(CaWriter* writer, 
				ndf_frame* frame, CcSemaphore* semframe);
		virtual ~ClTobiIdAsServer(void);
		virtual void HandleRecvPeer(CcSocket* caller, CcAddress addr, 
				CcStreamer* stream);
		void Register(CcServer* server);

	private:
		virtual bool CommunicationTiD(CcServer* server, CcAddress address, 
				CcStreamer* stream);
	protected:

	private:
		CaWriter* _writer;
		IDMessage _messageD;
		IDSerializerRapid *_serializerD;
		ndf_frame *_frame;
		CcSemaphore *_semframe;
		TCLanguage _tclang;
};

#endif
