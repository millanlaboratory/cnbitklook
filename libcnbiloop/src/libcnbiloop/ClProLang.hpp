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

#ifndef CLPROLANG_HPP 
#define CLPROLANG_HPP 

#include "ClLanguage.hpp" 
#include <libcnbicore/CcBasic.hpp>
#include <libcnbiconfig/CCfgTypes.hpp>
#include <string>

class ClProLang : public ClLanguage {
	public:
		virtual char* Fork(void);
		virtual char* Launch(const int pid, const std::string& call);
		virtual char* Terminate(const int pid);
		virtual char* IsAlive(const int pid);
		virtual char* Died(const int pid);
		virtual char* ChangeDirectory(const int pid, const std::string& path);
		virtual char* Include(const int pid, const std::string& path0, const std::string& path1);
		virtual char* LaunchNDF(const int pid, const std::string& function, 
				const std::string& pipename, const CcAddress addressD, 
				const CcAddress addressC, const std::string& extra);
		virtual char* Ok(const int pid);
		virtual char* Error(const int code);
		virtual bool IsFork(const char* message);
		virtual bool IsLaunch(const char* message, int* pid, std::string* call);
		virtual bool IsTerminate(const char* message, int* pid);
		virtual bool IsIsAlive(const char* message, int* pid);
		virtual bool IsDied(const char* message, int* pid);
		virtual bool IsOk(const char* message, int* pid);
		virtual bool IsError(const char* message, int* code);
		virtual bool IsChangeDirectory(const char* message, int* pid, std::string* path);
		virtual bool IsInclude(const char* message, int* pid, std::string* path0, 
				std::string* path1);
		virtual bool IsLaunchNDF(const char* message, int* pid, std::string* function,
				std::string* pipename, std::string* addressD, 
				std::string* addressC, std::string* extra);

	public:
		static const std::string Hdr;
		static const std::string Trl;

		static const int StatusLost = -3;
		static const int NotSupported = -2;
		static const int NotUndestood = -1;
		static const int NoReply = 0;
		static const int Successful = 1;
		static const int ForkFailed = 2;
		static const int IsDead = 3;
		static const int NotFound = 4;
};

#endif
