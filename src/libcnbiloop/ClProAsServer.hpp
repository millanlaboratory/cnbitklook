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

#ifndef CLPROASSERVER_HPP 
#define CLPROASSERVER_HPP 

#include "ClMatlab.hpp"
#include "ClProLang.hpp"
#include <libcnbicore/CcServerMulti.hpp>
#include <map>

class ClProAsServer : public CcSocketProxy {
	public:
		virtual void HandleBind(CcSocket* caller);
		virtual void HandleRelease(CcSocket* caller);
		virtual void HandleListen(CcSocket* caller); 
		virtual void HandleAcceptEndpoint(CcSocket* caller, CcAddress address);
		virtual void HandleDropEndpoint(CcSocket* caller, CcAddress address);
		virtual void HandleRecvEndpoint(CcSocket* caller, CcAddress address);
		virtual void Register(CcServerMulti* server);
	private:
		virtual int Fork(void);
		virtual void Remove(int pid);
		virtual ClMatlab* Get(int pid);
	protected:

	public:
	private:
		ClProLang language;
		std::map<int, ClMatlab*> _pool;
		CcSemaphore _sempool;
	protected:
};

#endif
