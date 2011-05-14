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

#ifndef CLNAMESLANG_HPP 
#define CLNAMESLANG_HPP 

#include "ClLanguage.hpp" 
#include <cnbicore/CcBasic.hpp>
#include <string>

class ClNamesLang : public ClLanguage {
	public:
		ClNamesLang(void);
		char* Query(const std::string& name);
		char* Reply(CcAddress address);
		char* Set(const std::string& name, CcAddress address);
		char* Unset(const std::string& name);
		char* Store(const std::string& name, const std::string& content);
		char* Retrieve(const std::string& name);
		char* Dispatch(const std::string& content);
		char* Erase(const std::string& name);

		char* Ok(void);
		char* Error(const int code);

		bool IsQuery(const char* message, std::string* name);
		bool IsReply(const char* message, CcAddress* address);
		bool IsSet(const char* message, std::string* name, CcAddress* address);
		bool IsUnset(const char* message, std::string* name);
		bool IsStore(const char* message, std::string* name, std::string* content);
		bool IsRetrieve(const char* message, std::string* name);
		bool IsDispatch(const char* message, std::string* content);
		bool IsErase(const char* message, std::string* name);
		bool IsOk(const char* message);
		bool IsError(const char* message, int* code);

		bool CheckName(const char* message);
	public:
		static const std::string Hdr;
		static const std::string Trl;
		static const int StatusLost = -4;
		static const int NotSupported = -3;
		static const int NotUndestood = -2;
		static const int NoReply = -1;
		static const int Successful = 0;
		static const int NotFound = 1;
		static const int AlreadySet = 2;
		static const int AlreadyStored= 3;
		static const int NotAvailable = 4;
		static const int NameFormatError = 5;
};

#endif
