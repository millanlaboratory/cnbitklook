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

/*! \brief Event logger
 *
 * This class, together with CcCore, provides one of the most important
 * components of libcnbicore. The user can add CcLogEntry objects to the logger,
 * that will serialize them on an XML file. Furthermore, CcLogger supports
 * coloured STDO (with Bash).
 */
class CcLogger : protected CcThread {
	public:
		CcLogger(bool syncdump = false, double writems = 250.00f);
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
		bool _syncdump;
};

#endif
