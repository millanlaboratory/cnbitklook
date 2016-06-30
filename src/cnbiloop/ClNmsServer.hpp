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
#ifndef CLNMSSERVER_HPP 
#define CLNMSSERVER_HPP 

#include "ClNmsLang.hpp"
#include <cnbicore/CcThread.hpp>
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcServer.hpp>
#include <cnbicore/CcSocketProxy.hpp>
#include <map>

class ClNmsServer : public CcSocketProxy, protected CcThread {
	public:
		virtual void HandleRecvPeer(CcSocket* caller, CcAddress addr, CcStreamer* stream);
		virtual void Register(CcServer* server);
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
		CcServer* _master;
		ClNmsLang language;
		std::map<std::string, CcAddress> _lookup;
		std::map<std::string, CcAddress> _monitor;
		std::map<std::string, std::string> _storage;
		CcSemaphore _semlookup;
		CcSemaphore _semmonitor;
		CcSemaphore _semstorage;
		double _monitorms;
};

#endif
