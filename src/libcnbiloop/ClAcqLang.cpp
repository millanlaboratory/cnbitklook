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

#ifndef CLACQLANG_CPP 
#define CLACQLANG_CPP 

#include "ClAcqLang.hpp" 
#include <stdio.h>
#include <string.h>

#define CLLA_OPEN_XDF_OUT		"[clla]open_xdf|%s|%s|%s[/clla]"
#define CLLA_OPEN_XDF_IN		"[clla]open_xdf|%[^'|']|%[^'|']|%[^'['][/clla]"
#define CLLA_CLOSE_XDF			"[clla]close_xdf[/clla]"
#define CLLA_ERROR				"[clla]error|%d[/clla]"
#define CLLA_OK					"[clla]ok[/clla]"

const std::string ClAcqLang::Hdr = "[clla]";
const std::string ClAcqLang::Trl = "[/clla]";

char* ClAcqLang::OpenXDF(const std::string& filegdf, 
		const std::string& filelog, const std::string& linelog) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLA_OPEN_XDF_OUT, filegdf.c_str(), filelog.c_str(),
			linelog.c_str());
	return ClLanguage::message->buffer;
}

char* ClAcqLang::CloseXDF(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLA_CLOSE_XDF);
	return ClLanguage::message->buffer;
}

char* ClAcqLang::Ok(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLA_OK); 
	return ClLanguage::message->buffer;
}

char* ClAcqLang::Error(const int code) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLA_ERROR, code);
	return ClLanguage::message->buffer;
}

bool ClAcqLang::IsOpenXDF(const char* message, std::string* filegdf, 
		std::string* filelog, std::string* linelog) {
	int count = sscanf(message, CLLA_OPEN_XDF_IN, 
			ClLanguage::_cache0->buffer,
			ClLanguage::_cache1->buffer, 
			ClLanguage::_cache2->buffer);
	if(count < 2)
		return false;

	filegdf->assign(ClLanguage::_cache0->buffer);
	filelog->assign(ClLanguage::_cache1->buffer);
	linelog->assign(ClLanguage::_cache2->buffer);
	return true;
}

bool ClAcqLang::IsCloseXDF(const char* message) {
	return(strcmp(CLLA_CLOSE_XDF, message) == 0);
}

bool ClAcqLang::IsOk(const char* message) {
	return(strcmp(CLLA_OK, message) == 0);
}

bool ClAcqLang::IsError(const char* message, int* code) {
	return(sscanf(message, CLLA_ERROR, code) == 1);
}

#endif
