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

#ifndef CCLOGGER_HPP
#define CCLOGGER_HPP

#include "CcCore.hpp"
#include "CcLogEntry.hpp"
#include "CcObject.hpp"
#include "CcSemaphore.hpp"
#include <fstream>
#include <iostream>
#include <string>


/*! \brief XML logger
 */
class CcLogger : public CcObject {
	public:
		CcLogger(void);
		~CcLogger(void);
		virtual void Open(std::string filename, std::string module, 
				CcTermType termtype, CcLogLevel level);
		virtual void Close(void);
		virtual bool IsOpen(void);
		virtual void AddEntry(CcLogEntry entry);
	private:
		virtual void DumpEntry(CcLogEntry* entry);

	private:
		CcSemaphore _sem;
		std::ofstream _file;
		bool _isopen;
		CcTermType _termtype;
		CcLogLevel _level;
};

#endif
