/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCHOST_HPP
#define CCHOST_HPP

#include "CcNetworkTypes.hpp"
#include "CcObject.hpp"
#include <libtransport/net.h>
#include <string>

typedef int CcEndpointRole;

/*! \brief TCP/UDP endpoint
 */
class CcEndpoint : public CcObject {
	public:
		CcEndpoint(void);
		CcEndpoint(const CcIp ip, const CcPort port);
		CcEndpoint(const CcIp ip, const unsigned int port);
		CcEndpoint(const CcAddress address);
		virtual ~CcEndpoint(void);

		virtual void Clear(void);

		virtual void Set(const tr_host *host, bool fixlocal = false);
		
		virtual void SetIp(const CcIp ip);
		virtual CcIp GetIp(void) const;

		virtual void SetPortUInt(const unsigned int port);
		virtual unsigned int GetPortUInt(void) const;
		
		virtual void SetPort(const CcPort port);
		virtual CcPort GetPort(void) const;
		
		virtual void SetAddress(const CcAddress address);
		virtual CcAddress GetAddress(void) const;
		
		virtual void SetRole(const CcEndpointRole role);
		virtual CcEndpointRole GetRole(void);
		
	private:
		virtual void Init(void);
		virtual void MakeAddress(const CcIp ip, const CcPort port);
		virtual void MakeAddress(const CcIp ip, const unsigned int port);
		virtual void MakeAddress(const tr_host *host);
		virtual void DecomposeAddress(const CcAddress address);

	public:
		static const CcEndpointRole RoleUndef =  0;
		static const CcEndpointRole RoleServer = 1;
		static const CcEndpointRole RoleClient = 2;
	private:
		CcIp _ip;
		CcAddress _address;
		CcPort _port;
		unsigned int _iport;
		CcEndpointRole _role;
};

#endif
