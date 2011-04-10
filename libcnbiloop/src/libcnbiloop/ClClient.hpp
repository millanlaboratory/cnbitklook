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
	protected:
		virtual void HandleDisconnect(CcSocket* caller);

	public:
	private:
	protected:
		std::stringstream _stream;
		CcSemaphore _semrecv;
		CcClient _client;
		double _waitms;
};

#endif
