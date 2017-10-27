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

#ifndef CLTOBIID_HPP 
#define CLTOBIID_HPP 

#include "ClLoop.hpp"
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcClientExt.hpp>
#include <cnbicore/CcCallback.hpp>
#include <cnbicore/CcProxy.hpp>
#include <tobicore/TCException.hpp>
#include <tobicore/TCLanguage.hpp>
#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>
#include <list>

class ClTobiId : public CcSocketProxy {
	public:
		ClTobiId(int mode = ClTobiId::SetOnly, float waitms = 3.00);
		virtual ~ClTobiId(void);
		bool Attach(const std::string& name = "/bus");
		bool Detach(void);
		bool IsAttached(void);
		bool GetMessage(IDSerializerRapid* serializer);
		bool SetMessage(IDSerializerRapid* serializer, 
				int blockidx = TCBlock::BlockIdxUnset, int* fixd = NULL);
		int Count(void);
		int Clear(void);
		// Created by L.Tonin  <luca.tonin@epfl.ch> on 09/06/17 10:54:42
		int GetMode(void);
	protected:
		void HandleConnect(CcSocket* caller);
		void HandleDisconnect(CcSocket* caller);
		void HandleRecv(CcSocket* caller, CcStreamer* stream);

	public:
		static const int SetOnly = 0;
		static const int GetOnly = 1;
		static const int SetGet = 2;
		CcCallback0<CcProxy> iOnAttach;
		CcCallback0<CcProxy> iOnDetach;
		CcCallback0<CcProxy> iOnHasMessage;

	protected:
		CcClientExt* _client;
		std::string _name;
		std::string _buffer;
		CcSemaphore _sembuffer;
		
		CcSemaphore _semqueue;
		std::list<std::string> _queue;
		int _mode;
		TCLanguage _tclang;
		float _waitms;
};

#endif
