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

#ifndef CCLOGENTRY_HPP
#define CCLOGENTRY_HPP

#include "CcCore.hpp"
#include <string>
#include <sstream>
class CcLogger;

/*! \brief Log entry for CcLogger
 *
 * CcLogger is a threaded logger that logs both to file and to STDO. Objects of
 * the CcLogEntry class can be passed to the logger and are internally used to
 * handle the log entry cues.
 */
class CcLogEntry {
	friend class CcLogger;

	public:
		CcLogEntry(CcLogLevel level, std::string message, std::string caller, 
				std::string file, int line);
		CcLogEntry(CcLogLevel level, std::stringstream message, std::string caller, 
				std::string file, int line);
		virtual ~CcLogEntry(void);
		virtual std::string Serialize(void);
		virtual CcLogLevel GetLevel(void);
		static const std::string GetLevelTxt(CcLogLevel level);
	public:
		static const std::string TxtDebug;
		static const std::string TxtConfig;
		static const std::string TxtInfo;
		static const std::string TxtException;
		static const std::string TxtWarning;
		static const std::string TxtError;
		static const std::string TxtFatal;
	protected:
		std::string _timestamp;
		std::string _caller;
		std::string _message;
		std::string _file;
		int _line;
		CcLogLevel _level;

};

#endif
