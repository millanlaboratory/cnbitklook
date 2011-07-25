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

#ifndef CLPROASSERVER_HPP 
#define CLPROASSERVER_HPP 

#include "ClMatlab.hpp"
#include "ClProLang.hpp"
#include <cnbicore/CcServer.hpp>
#include <cnbicore/CcSocketProxy.hpp>
#include <map>

class ClProAsServer : public CcSocketProxy {
	public:
		virtual void HandleRecvPeer(CcSocket* caller, CcAddress addr,
				CcStreamer* stream);
		virtual void Register(CcServer* server);
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
