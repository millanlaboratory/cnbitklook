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

#ifndef CLLOOPCONFIG_HPP 
#define CLLOOPCONFIG_HPP 

#include "ClLoop.hpp"
#include <cnbicore/CcNetworkTypes.hpp>

class ClLoopConfig {
	friend class ClLoop;

	public:
		static ClLoopConfig* Instance(void);
		static void Release(void);
		static unsigned int Refcount(void);
		static CcIp GetIp(void);
		static CcAddress GetNms(void);
		static CcAddress GetPro(void);
		static CcAddress GetAcq(void);
		static CcAddress GetBus(void);
		static CcAddress GetDev(void);
	protected:
		static void Init(void);
		ClLoopConfig(void);
		virtual ~ClLoopConfig(void);
		static void Destroy(void);
		static void Read(const std::string& filename);
		static void ImplLoad(void);
		
	protected:
		static CcIp ip;
		static CcPort portNms;
		static CcPort portPro;
		static CcPort portAcq;
		static CcPort portBus;
		static CcPort portDev;
	private:
		static ClLoopConfig* _instance;
		static unsigned int _refCount;
		static bool _loaded;
};

#endif
