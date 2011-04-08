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

#ifndef CCCORE_HPP
#define CCCORE_HPP

#include <iostream>
#include <string>
//#include <list>
#include "CcLogEntry.hpp"
#include "CcNetworkTypes.hpp"

typedef int CcTermType;
class CcObject;
class CcLogger;
template <class T> class CcThreadSafe;
/*! \brief Singleton kernel class
 *
 * This class provides core functionalities to all the classes in libcnbicore
 */
class CcCore {
	public:
		static CcCore* Instance(void);
		static void Release(void);
		static unsigned int Refcount(void);
		static void Status(void);
		static void OpenLogger(std::string module,
				CcTermType termtype = CcCore::TerminalColors,
				CcLogLevel level = CcLogEntry::LevelConfig);
		static std::string GetDirectoryTmp(void);
		static std::string GetDirectoryCwd(void);
		static std::string GetDirectoryHome(void);
		static std::string GetUsername(void);
		static void CatchSIGINT(void);
		static void CatchSIGQUIT(void);
		static void CatchSIGTERM(void);
		static void CatchSIGCHLD(void);

	protected:
		CcCore(void);
		~CcCore(void);
		static void Destroy(void);

	public:
		static CcLogger logger;
		static std::string username;
		static std::string directory;
		static const CcTermType TerminalDisabled = 0;
		static const CcTermType TerminalEnabled = 1;
		static const CcTermType TerminalColors = 2;
		static CcThreadSafe<bool> receivedSIGINT;
		static CcThreadSafe<bool> receivedSIGQUIT;
		static CcThreadSafe<bool> receivedSIGTERM;
		static CcThreadSafe<bool> receivedSIGCHLD;
	private:
		static CcCore* _instance;
		static unsigned int _refCount;

};
#endif
