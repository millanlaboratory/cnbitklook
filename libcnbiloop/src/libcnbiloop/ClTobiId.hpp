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

#ifndef CLTOBIID_HPP 
#define CLTOBIID_HPP 

#include "ClLoop.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcClient.hpp>
#include <libtobicore/TCException.hpp>
#include <libtobiid/IDMessage.hpp>
#include <libtobiid/IDSerializerRapid.hpp>
#include <list>

class ClTobiId : public CcSocketProxy {
	public:
		ClTobiId(void);
		virtual ~ClTobiId(void);
		virtual bool Attach(const std::string& name = "/acquisition");
		virtual bool Detach(void);
		virtual bool IsAttached(void);
		virtual bool GetMessage(IDSerializerRapid* serializer);
		virtual bool SendMessage(IDSerializerRapid* serializer);
		virtual int Count(void);
	protected:
		virtual void HandleConnect(CcSocket* caller);
		virtual void HandleDisconnect(CcSocket* caller);
		virtual void HandleRecv(CcSocket* caller);

	public:
	protected:
		CcClient* _client;
		std::string _name;
		std::string _buffer;
		CcSemaphore _sembuffer;
		
		CcSemaphore _semqueue;
		std::list<std::string> _queue;
};

#endif
