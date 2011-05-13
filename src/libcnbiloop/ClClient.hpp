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

#ifndef CLCLIENT_HPP 
#define CLCLIENT_HPP 

#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcClient.hpp>

class ClClient : public CcSocketProxy {
	public:
		ClClient(const double waitms = 5000.00f);
		virtual ~ClClient(void);
		virtual bool Connect(const CcAddress address);
		virtual void Disconnect(void);
		virtual bool IsConnected(void);
	protected:
		virtual void HandleDisconnect(CcSocket* caller);

	public:
	private:
	protected:
		CcSemaphore _semrecv;
		CcClient _client;
		double _waitms;
};

#endif
