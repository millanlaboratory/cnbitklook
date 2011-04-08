/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLNAMESLANG_HPP 
#define CLNAMESLANG_HPP 

#include "ClLanguage.hpp" 
#include <libcnbicore/CcBasic.hpp>
#include <string>

class ClNamesLang : public ClLanguage {
	public:
		char* Query(const std::string& name);
		char* Reply(CcAddress address);
		char* Set(const std::string& name, CcAddress address);
		char* Unset(const std::string& name);
		char* Store(const std::string& name, const std::string& content);
		char* Retrieve(const std::string& name);
		char* Dispatch(const std::string& name, const std::string& content);

		char* Ok(void);
		char* Error(const int code);

		bool IsQuery(const char* message, std::string* name);
		bool IsReply(const char* message, CcAddress* address);
		bool IsSet(const char* message, std::string* name, CcAddress* address);
		bool IsUnset(const char* message, std::string* name);
		bool IsOk(const char* message);
		bool IsError(const char* message, int* code);
	public:
		static const std::string Hdr;
		static const std::string Trl;
		static const int StatusLost = -3;
		static const int NotSupported = -2;
		static const int NotUndestood = -1;
		static const int NoReply = 0;
		static const int Successful = 1;
		static const int NotFound = 2;
		static const int AlreadySet = 3;
};

#endif
