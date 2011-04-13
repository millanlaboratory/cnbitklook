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

#ifndef CLNAMESASSERVER_HPP 
#define CLNAMESASSERVER_HPP 

#include "ClNamesLang.hpp"
#include <libcnbicore/CcThread.hpp>
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcServerMulti.hpp>
#include <map>

class ClNamesAsServer : public CcSocketProxy, protected CcThread {
	public:
		virtual void HandleBind(CcSocket* caller);
		virtual void HandleRelease(CcSocket* caller);
		virtual void HandleListen(CcSocket* caller); 
		virtual void HandleAcceptEndpoint(CcSocket* caller, CcAddress address);
		virtual void HandleDropEndpoint(CcSocket* caller, CcAddress address);
		virtual void HandleRecvEndpoint(CcSocket* caller, CcAddress address);
		virtual void Register(CcServerMulti* server);
		virtual void StartMonitor(double waitms = 60000.00f);
		virtual void StopMonitor(void);
		virtual bool IsMonitoring(void);
	protected:
		virtual bool Get(const std::string& name, CcAddress* address);
		virtual bool Set(const std::string& name, const CcAddress address);
		virtual bool Unset(const std::string& name);
		virtual void Main(void);
		virtual void AddMonitored(const std::string& name, CcAddress address);
		virtual bool Retrieve(const std::string& name, CcAddress* address);
		virtual bool Store(const std::string& name, const std::string& address);
		virtual bool Erase(const std::string& name);
	
	public:
	private:
		CcServerMulti* _master;
		ClNamesLang language;
		std::stringstream _stream;
		std::map<std::string, CcAddress> _lookup;
		std::map<std::string, CcAddress> _monitor;
		std::map<std::string, std::string> _storage;
		CcSemaphore _semlookup;
		CcSemaphore _semmonitor;
		CcSemaphore _semstorage;
		double _monitorms;
};

#endif
