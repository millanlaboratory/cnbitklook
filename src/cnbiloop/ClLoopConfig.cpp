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

#ifndef CLLOOPCONFIG_CPP 
#define CLLOOPCONFIG_CPP 

#include "ClLoopConfig.hpp" 
#include <cnbicore/CcCore.hpp> 
#include <cnbicore/CcLog.hpp> 
#include <sys/stat.h>
#include <fstream>

/* Initialization */
ClLoopConfig* ClLoopConfig::_instance = NULL;
unsigned int ClLoopConfig::_refCount = 0;
bool ClLoopConfig::_loaded = false;
std::string ClLoopConfig::matlabBinary = "";
std::string ClLoopConfig::matlabVariant = "";

CcPort ClLoopConfig::ip;
CcPort ClLoopConfig::portNms;
CcPort ClLoopConfig::portPro;
CcPort ClLoopConfig::portAcq;
CcPort ClLoopConfig::portBus;
CcPort ClLoopConfig::portDev;

ClLoopConfig::ClLoopConfig(void) {
}

ClLoopConfig::~ClLoopConfig(void) {
}

ClLoopConfig* ClLoopConfig::Instance(void) {
	if(ClLoopConfig::_instance == NULL)
		ClLoopConfig::_instance = new ClLoopConfig;
	++ClLoopConfig::_refCount;
	return ClLoopConfig::_instance;
}

unsigned int ClLoopConfig::Refcount(void) {
	return ClLoopConfig::_refCount;
}

void ClLoopConfig::Release(void) {
	if(--ClLoopConfig::_refCount < 1) 
		ClLoopConfig::Destroy();
}

void ClLoopConfig::Destroy(void) {
	if(ClLoopConfig::_instance == NULL) 
		return;
	delete ClLoopConfig::_instance;
}

void ClLoopConfig::ImplLoad(void) {
	struct stat st;
	std::string etc = "/etc/cnbitk/cnbiloop";
	std::string home = CcCore::GetDirectoryHome() + "/.cnbiloop";

	if(stat(home.c_str(), &st) == 0) {
		CcLogConfigS("Loading user configuration file: " << home);
		ClLoopConfig::Read(home);
	} else if(stat(etc.c_str(), &st) == 0) {
		CcLogConfigS("Loading system configuration file: " << etc);
		ClLoopConfig::Read(etc);
	} else {
		CcLogConfigS("Using default configuration");
		ClLoopConfig::Init();
	}
	ClLoopConfig::_loaded = true;
}

void ClLoopConfig::Init(void) {
	ClLoopConfig::ip = "127.0.0.1";
	ClLoopConfig::portNms = "8123";
	ClLoopConfig::portPro = "8124";
	ClLoopConfig::portAcq = "8125";
	ClLoopConfig::portBus = "8126";
	ClLoopConfig::portDev = "8127";
}
		
void ClLoopConfig::Read(const std::string& filename) {
	if(ClLoopConfig::_loaded == true)
		return;

	std::ifstream file(filename.c_str());
	std::string cache;
	char trash[2048];
	char buffer[2048];
	int line = 0;

	while(std::getline(file, cache)) {
		++line;
		if(cache.find("#") != std::string::npos)
			continue;

		if(cache.find("loop.ip") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::ip.assign(buffer);
				continue;
			}
		} else if(cache.find("nms.port") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::portNms.assign(buffer);
				continue;
			}
		} else if(cache.find("acq.port") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::portAcq.assign(buffer);
				continue;
			}
		} else if(cache.find("pro.port") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::portPro.assign(buffer);
				continue;
			}
		} else if(cache.find("dev.port") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::portDev.assign(buffer);
				continue;
			}
		} else if(cache.find("bus.port") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::portBus.assign(buffer);
				continue;
			}
		} else if(cache.find("matlab.binary") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::matlabBinary.assign(buffer);
				continue;
			}
		} else if(cache.find("matlab.variant") != std::string::npos) {
			if(sscanf(cache.c_str(), "%s = %s", trash, buffer) == 2) {
				ClLoopConfig::matlabVariant.assign(buffer);
				continue;
			}
		} else 
			continue;
		CcLogErrorS("Bad format: " << filename << ", line=" << line);
	}
}

CcAddress ClLoopConfig::GetNms(void) {
	CcAddress address = ClLoopConfig::ip + ":" + ClLoopConfig::portNms;
	return address;
}

CcAddress ClLoopConfig::GetPro(void) {
	CcAddress address = ClLoopConfig::ip + ":" + ClLoopConfig::portPro;
	return address;
}

CcAddress ClLoopConfig::GetAcq(void) {
	CcAddress address = ClLoopConfig::ip + ":" + ClLoopConfig::portAcq;
	return address;
}

CcAddress ClLoopConfig::GetBus(void) {
	CcAddress address = ClLoopConfig::ip + ":" + ClLoopConfig::portBus;
	return address;
}

CcAddress ClLoopConfig::GetDev(void) {
	CcAddress address = ClLoopConfig::ip + ":" + ClLoopConfig::portDev;
	return address;
}

CcIp ClLoopConfig::GetIp(void) {
	return ClLoopConfig::ip;
}

#endif
