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

#ifndef CLLOOP_HPP 
#define CLLOOP_HPP 

#include "ClProClient.hpp"
#include "ClAcqClient.hpp"
#include "ClNamesClient.hpp"
#include <cnbicore/CcBasic.hpp>
#include <cnbiconfig/CCfgConfig.hpp>

class ClLoop {
	public:
		static ClLoop* Instance(void);
		static void Release(void);
		static unsigned int Refcount(void);

		static bool Connect(void);
		static bool Connect(CcAddress nameserver);
		static bool Connect(CCfgConfig* configuration);
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
	private:
		static ClLoop* _instance;
		static unsigned int _refCount;
};

#endif
