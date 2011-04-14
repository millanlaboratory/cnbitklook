/*
    Copyright (C) 2011 Michele Tavella <tavella.michele@gmail.com>

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

#ifndef CLLOOP_HPP 
#define CLLOOP_HPP 

#include "ClProClient.hpp"
#include "ClAcqClient.hpp"
#include "ClNamesClient.hpp"
#include <libcnbicore/CcBasic.hpp>

class ClLoop {
	public:
		static ClLoop* Instance(void);
		static void Release(void);
		static unsigned int Refcount(void);

		static bool Connect(CcAddress nameserver = "127.0.0.1:8000");
		static void Disconnect(void);
		static bool IsConnected(void);
	protected:
		ClLoop(void);
		virtual ~ClLoop(void);
		static void Destroy(void);
		static bool ConnectNameserver(void);
		static bool ConnectProcessing(void);
		static bool ConnectAcquisition(void);
		static bool QueryAddresses(void);

	public:
		static ClProClient processing;
		static ClAcqClient acquisition;
		static ClNamesClient nameserver;
	protected:
		static CcAddress _processing;
		static CcAddress _acquisition;
		static CcAddress _nameserver;
		static std::stringstream _stream;
	private:
		static ClLoop* _instance;
		static unsigned int _refCount;
};

#endif
