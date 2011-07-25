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

#ifndef CLACQSERVER_HPP 
#define CLACQSERVER_HPP 

#include "ClAcqLang.hpp"
#include <cnbiacq/CaWriter.hpp>
#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcServer.hpp>
#include <cnbicore/CcSocketProxy.hpp>

class ClAcqServer : public CcSocketProxy {
	public:
		ClAcqServer(CaWriter* writer);
		virtual ~ClAcqServer(void);

		virtual void HandleRecvPeer(CcSocket* caller, CcAddress addr, CcStreamer* stream);
		void Register(CcServer* server);

	private:
		virtual bool CommunicationCl(CcServer* server, CcAddress address, 
				CcStreamer* stream);
		virtual bool CreateLog(const std::string& logfile, const std::string& xdffile, 
				const std::string& logline); 
		virtual bool UpdateLog(const std::string& logfile,  
				const std::string& logline, bool space = false); 
	protected:

	public:
		ClAcqLang language;
	private:
		CaWriter* _writer;
		std::string _log;
		std::string _xdf;
};

#endif
