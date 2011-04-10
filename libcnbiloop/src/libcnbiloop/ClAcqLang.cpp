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

#ifndef CLACQLANG_CPP 
#define CLACQLANG_CPP 

#include "ClAcqLang.hpp" 
#include <stdio.h>
#include <string.h>

#define CLLA_ADD_LABEL_GDF		"[clla]add_gdf|%u[/clla]"
#define CLLA_ADD_LABEL_LPT		"[clla]add_lpt|%u[/clla]"
#define CLLA_ADD_LABEL_TXT		"[clla]add_txt|%s[/clla]"
#define CLLA_OPEN_XDF_OUT		"[clla]open_xdf|%s|%s|%s[/clla]"
#define CLLA_OPEN_XDF_IN		"[clla]open_xdf|%[^'|']|%[^'|']|%[^'['][/clla]"
#define CLLA_CLOSE_XDF			"[clla]close_xdf[/clla]"
#define CLLA_ERROR				"[clla]error|%d[/clla]"
#define CLLA_OK					"[clla]ok[/clla]"

const std::string ClAcqLang::Hdr = "[clla]";
const std::string ClAcqLang::Trl = "[/clla]";

char* ClAcqLang::AddLabelGDF(const GDFEvent label) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MaxSize(),
			CLLA_ADD_LABEL_GDF, label);
	return ClLanguage::message->buffer;
}

char* ClAcqLang::AddLabelLPT(const HWTrigger label) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MaxSize(),
			CLLA_ADD_LABEL_LPT, label);
	return ClLanguage::message->buffer;
}

char* ClAcqLang::AddLabelTXT(const std::string& label) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MaxSize(),
			CLLA_ADD_LABEL_TXT, label.c_str());
	return ClLanguage::message->buffer;
}

char* ClAcqLang::OpenXDF(const std::string& filegdf, 
		const std::string& filelog, const std::string& linelog) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MaxSize(),
			CLLA_OPEN_XDF_OUT, filegdf.c_str(), filelog.c_str(),
			linelog.c_str());
	return ClLanguage::message->buffer;
}

char* ClAcqLang::CloseXDF(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MaxSize(),
			CLLA_CLOSE_XDF);
	return ClLanguage::message->buffer;
}

char* ClAcqLang::Ok(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MaxSize(),
			CLLA_OK); 
	return ClLanguage::message->buffer;
}

char* ClAcqLang::Error(const int code) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MaxSize(),
			CLLA_ERROR, code);
	return ClLanguage::message->buffer;
}

bool ClAcqLang::IsAddLabelGDF(const char* message, GDFEvent* label) {
	return(sscanf(message, CLLA_ADD_LABEL_GDF, label) == 1);
}

bool ClAcqLang::IsAddLabelLPT(const char* message, HWTrigger* label) {
	return(sscanf(message, CLLA_ADD_LABEL_LPT, (unsigned int*)label) == 1);
}

bool ClAcqLang::IsAddLabelTXT(const char* message, std::string* label) {
	if(sscanf(message, CLLA_ADD_LABEL_TXT, ClLanguage::_cache0->buffer) != 1)
		return false;
	label->assign(ClLanguage::_cache0->buffer);
	return true;
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
