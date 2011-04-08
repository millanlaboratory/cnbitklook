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
#ifndef CCEXCEPTION_HPP
#define CCEXCEPTION_HPP

#include "CcLogEntry.hpp"
#include "CcObject.hpp"
#include <string>

/*! \brief Exception class
 * 
 * Implements a simple exception with functionalities to help the user
 * unserstanding which piece of code was kicked by Chuck Norris.
 */
class CcException : public CcObject {
	public:
		CcException(std::string info, std::string caller = "undef");
		//	__attribute__ ((deprecated));
		virtual ~CcException(void);
		virtual std::string Caller(void) const;
		virtual std::string Info(void) const;
		virtual std::string Message(void) const;
	private:
		virtual void CreateMessage(void);
	public:
		friend std::ostream &operator<<(std::ostream& output, 
				const CcException&);
		friend std::ostream &operator<<(std::ostream& output, 
				const CcException*);
		bool operator==(const CcException& right);
		bool operator!=(const CcException& right);

	private:
		std::string _info;
		std::string _caller;
		std::string _message;
};

#define CcThrow(message)\
	{\
	CcException exception(message, __PRETTY_FUNCTION__); \
	CcCore::logger.AddEntry(CcLogEntry(CcLogEntry::LevelException,\
				exception.Info(),\
				exception.Caller(),\
				__FILE__, __LINE__)); \
	throw exception;\
	}
#endif
