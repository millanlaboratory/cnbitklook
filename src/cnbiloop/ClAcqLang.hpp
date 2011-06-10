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

#ifndef CLACQLANG_HPP 
#define CLACQLANG_HPP 

#include "ClLanguage.hpp" 
#include <cnbiconfig/CCfgTypes.hpp>
#include <string>

class ClAcqLang : public ClLanguage {
	public:
		char* OpenXDF(const std::string& filegdf, 
				const std::string& filelog, 
				const std::string& linelog);
		char* CloseXDF(void);
		char* UpdateLog(const std::string& linelog);
		char* Ok(void);
		char* Error(const int code);
		bool IsOpenXDF(const char* message, 
				std::string* filegdf,
				std::string* filelog, 
				std::string* linelog);
		bool IsCloseXDF(const char* message);
		bool IsUpdateLog(const char* message, std::string* linelog);
		bool IsOk(const char* message);
		bool IsError(const char* message, int* code);

	public:
		static const std::string Hdr;
		static const std::string Trl;
		static const int StatusLost = -4;
		static const int NotSupported = -3;
		static const int NotUndestood = -2;
		static const int NoReply = -1;
		static const int Successful = 0;
		static const int NDFLimitReached = 1;
		static const int XDFAlreadyOpen = 2;
		static const int XDFOpenFailed = 3;
		static const int LogOpenFailed = 4;
		static const int XDFNotOpen = 5;
		static const int XDFCloseFailed = 6;
		static const int XDFSetupFailed = 7;
		static const int LogUpdateFailed = 8;
};

#endif
