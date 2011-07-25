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

#ifndef CLLOOPCONFIGSRV_CPP 
#define CLLOOPCONFIGSRV_CPP 

#include "ClLoopConfigSrv.hpp" 

CcPort ClLoopConfigSrv::ipSrv;
		
void ClLoopConfigSrv::LoadSrv(void) {
	ClLoopConfig::Load();
	if(ClLoopConfig::ip.compare("0.0.0.0") == 0) {
		ClLoopConfigSrv::ip.assign("127.0.0.1");	
		ClLoopConfigSrv::ipSrv.assign("0.0.0.0");	
	}
}

CcAddress ClLoopConfigSrv::GetSrvNms(void) {
	CcAddress address = ClLoopConfigSrv::ipSrv + ":" + ClLoopConfig::portNms;
	return address;
}

CcAddress ClLoopConfigSrv::GetSrvPro(void) {
	CcAddress address = ClLoopConfigSrv::ipSrv + ":" + ClLoopConfig::portPro;
	return address;
}

CcAddress ClLoopConfigSrv::GetSrvAcq(void) {
	CcAddress address = ClLoopConfigSrv::ipSrv + ":" + ClLoopConfig::portAcq;
	return address;
}

CcAddress ClLoopConfigSrv::GetSrvBus(void) {
	CcAddress address = ClLoopConfigSrv::ipSrv + ":" + ClLoopConfig::portBus;
	return address;
}

CcAddress ClLoopConfigSrv::GetSrvDev(void) {
	CcAddress address = ClLoopConfigSrv::ipSrv + ":" + ClLoopConfig::portDev;
	return address;
}

CcIp ClLoopConfigSrv::GetSrvIp(void) {
	return ClLoopConfig::ip;
}


#endif
