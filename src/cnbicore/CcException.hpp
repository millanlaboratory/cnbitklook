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

#ifndef CCEXCEPTION_HPP
#define CCEXCEPTION_HPP

#include "CcCore.hpp"
#include "CcObject.hpp"
#include <string>

/*! \brief Exception
 * 
 * Implements an exeption and provides the method to set the calling function,
 * an information string and the exception message.
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
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelException,\
				exception.Info(),\
				exception.Caller(),\
				__FILE__, __LINE__)); \
	throw exception;\
	}
#endif
