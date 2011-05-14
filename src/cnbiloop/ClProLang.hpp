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

#ifndef CLPROLANG_HPP 
#define CLPROLANG_HPP 

#include "ClLanguage.hpp" 
#include <cnbicore/CcBasic.hpp>
#include <cnbiconfig/CCfgTypes.hpp>
#include <string>

class ClProLang : public ClLanguage {
	public:
		char* Fork(void);
		char* Launch(const int pid, const std::string& call);
		char* Terminate(const int pid);
		char* IsAlive(const int pid);
		char* Died(const int pid);
		char* ChangeDirectory(const int pid, const std::string& path);
		char* Include(const int pid, const std::string& path0, const std::string& path1);
		char* LaunchNDF(const int pid, const std::string& function, 
				const std::string& pipename, const CcAddress iD, 
				const CcAddress iC);
		char* Ok(const int pid);
		char* Error(const int code);
		bool IsFork(const char* message);
		bool IsLaunch(const char* message, int* pid, std::string* call);
		bool IsTerminate(const char* message, int* pid);
		bool IsIsAlive(const char* message, int* pid);
		bool IsDied(const char* message, int* pid);
		bool IsOk(const char* message, int* pid);
		bool IsError(const char* message, int* code);
		bool IsChangeDirectory(const char* message, int* pid, std::string* path);
		bool IsInclude(const char* message, int* pid, std::string* path0, 
				std::string* path1);
		bool IsLaunchNDF(const char* message, int* pid, std::string* function,
				std::string* pipename, std::string* iD, 
				std::string* iC);

	public:
		static const std::string Hdr;
		static const std::string Trl;
		static const int StatusLost = -4;
		static const int NotSupported = -3;
		static const int NotUndestood = -2;
		static const int NoReply = -1;
		static const int Successful = 0;
		static const int ForkFailed = 1;
		static const int IsDead = 2;
		static const int NotFound = 3;
};

#endif
