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

#ifndef CCHOST_HPP
#define CCHOST_HPP

#include "CcNetworkTypes.hpp"
#include "CcObject.hpp"
#include <transport/tr_net.h>
#include <string>

/*! \brief TCP/UDP endpoint
 *
 * This class represents a TCP/UDP connection endpoint, and can be used both by
 * a variety of classes such as CcServerSingle and CcServerMulti.
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
	private:
		virtual void MakeAddress(const CcIp ip, const CcPort port);
		virtual void MakeAddress(const CcIp ip, const unsigned int port);
		virtual void MakeAddress(const tr_host *host);
		virtual void DecomposeAddress(const CcAddress address);

	private:
		CcIp _ip;
		CcAddress _address;
		CcPort _port;
		unsigned int _iport;
};

#endif
