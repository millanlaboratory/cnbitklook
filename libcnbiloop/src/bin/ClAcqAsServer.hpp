/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLACQASSERVER_HPP 
#define CLACQASSERVER_HPP 

#include "ClAcqLang.hpp"
#include <libcnbiacq/CaWriter.hpp>
#include <libtobiid/IDMessage.hpp>
#include <libtobiid/IDSerializerRapid.hpp>
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcServerMulti.hpp>

class ClAcqAsServer  : public CcSocketProxy {
	public:
		ClAcqAsServer(CaWriter* writer, 
				ndf_frame* frame, CcSemaphore* semframe);
		virtual ~ClAcqAsServer(void);

		virtual void HandleBind(CcSocket* caller);
		virtual void HandleRelease(CcSocket* caller);
		virtual void HandleListen(CcSocket* caller); 
		virtual void HandleAcceptEndpoint(CcSocket* caller, CcAddress address);
		virtual void HandleDropEndpoint(CcSocket* caller, CcAddress address);
		virtual void HandleRecvEndpoint(CcSocket* caller, CcAddress address);
		void Register(CcServerMulti* server);

	private:
		virtual bool CommunicationCl(CcServerMulti* server, CcAddress address);
		virtual bool CommunicationTiD(CcServerMulti* server, CcAddress address);
		virtual bool LogXDF(const std::string& logfile, const std::string& xdffile, 
				const std::string& logline); 
	protected:

	public:
		ClAcqLang language;
	private:
		CaWriter* _writer;
		IDMessage _messageD;
		IDSerializerRapid *_serializerD;
		ndf_frame *_frame;
		CcSemaphore *_semframe;
		std::stringstream _stream;
	protected:
};

#endif
