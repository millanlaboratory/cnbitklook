/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libxmlconfig library

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

#ifndef XMLEXCEPTION_HPP
#define XMLEXCEPTION_HPP

#include <string>

//! TODO
class XMLException {
	public:
		XMLException(std::string info, std::string caller);
		virtual ~XMLException(void);

		virtual std::string Caller(void) const;
		virtual std::string Info(void) const;
		
	public:
		friend std::ostream &operator<<(std::ostream& output, 
				const XMLException&);
		friend std::ostream &operator<<(std::ostream& output, 
				const XMLException*);
		bool operator==(const XMLException& right);
		bool operator!=(const XMLException& right);

	private:
		std::string _info;
		std::string _caller;
		std::string _extra;
};

#endif
