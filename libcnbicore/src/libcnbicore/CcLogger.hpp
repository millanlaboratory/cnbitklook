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
#include "CcThread.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <list>

/*! \brief XML logger
 */
class CcLogger : protected CcThread {
	public:
		CcLogger(double writems = 5000.00f);
		virtual ~CcLogger(void);
		void Open(const std::string& filename, const std::string& module, 
				const CcTermType termtype, const CcLogLevel level);
		void Close(void);
		bool IsOpen(void);
		void AddEntry(CcLogEntry entry);
	private:
		virtual void WriteEntry(CcLogEntry* entry);
		virtual void DumpEntry(CcLogEntry* entry);
		void Main(void);
		void SwitchList(void);

	private:
		CcTermType _termtype;
		CcLogLevel _level;
		std::ofstream _filexml;
		CcSemaphore _semxml;
		std::list<CcLogEntry> _list1;
		std::list<CcLogEntry> _list2;
		std::list<CcLogEntry>* _listW;
		std::list<CcLogEntry>* _listR;
		CcSemaphore _semlist;
		double _writems;
};

#endif
