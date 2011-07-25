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

#ifndef CLLOOPCONFIGSRV_HPP 
#define CLLOOPCONFIGSRV_HPP 

#include "ClLoopConfig.hpp" 

class ClLoopConfigSrv : public ClLoopConfig {
	public:
		static void LoadSrv(void);
		static CcIp GetSrvIp(void);
		static CcAddress GetSrvNms(void);
		static CcAddress GetSrvPro(void);
		static CcAddress GetSrvAcq(void);
		static CcAddress GetSrvBus(void);
		static CcAddress GetSrvDev(void);

	protected:
		static CcIp ipSrv;
};

#endif
