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

#ifndef CLACQLANG_HPP 
#define CLACQLANG_HPP 

#include "ClLanguage.hpp" 
#include <libcnbiconfig/CCfgTypes.hpp>
#include <string>

class ClAcqLang : public ClLanguage {
	public:
		 virtual char* AddLabelGDF(const GDFEvent label);
		 virtual char* AddLabelLPT(const HWTrigger label);
		 virtual char* AddLabelTXT(const std::string& label);
		 virtual char* OpenXDF(const std::string& filegdf, const std::string&
				 filelog, const std::string& linelog);
		 virtual char* CloseXDF(void);
		 virtual char* Ok(void);
		 virtual char* Error(const int code);
		 virtual bool IsAddLabelGDF(const char* message, GDFEvent* label);
		 virtual bool IsAddLabelLPT(const char* message, HWTrigger* label);
		 virtual bool IsAddLabelTXT(const char* message, std::string* label);
		 virtual bool IsOpenXDF(const char* message, std::string* filegdf,
				 std::string* filelog, std::string* linelog);
		 virtual bool IsCloseXDF(const char* message);
		 virtual bool IsOk(const char* message);
		 virtual bool IsError(const char* message, int* code);

	public:
		static const std::string Hdr;
		static const std::string Trl;
		static const int StatusLost = -3;
		static const int NotSupported = -2;
		static const int NotUndestood = -1;
		static const int NoReply = 0;
		static const int Successful = 1;
		static const int NDFLimitReached = 5;
		static const int XDFAlreadyOpen = 6;
		static const int XDFOpenFailed = 7;
		static const int LogOpenFailed = 8;
		static const int XDFNotOpen = 9;
		static const int XDFCloseFailed = 10;
		static const int XDFSetupFailed = 11;
};

#endif
