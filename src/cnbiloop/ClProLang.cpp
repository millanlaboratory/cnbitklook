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

#ifndef CLPROLANG_CPP 
#define CLPROLANG_CPP 

#include "ClProLang.hpp" 
#include <stdio.h>

#define CLLP_FORK			"[cllp]fork[/cllp]"
#define CLLP_EXEC_OUT		"[cllp]exec|%d|%s[/cllp]"
#define CLLP_EXEC_IN		"[cllp]exec|%d|%[^'['][/cllp]"
#define CLLP_TERMINATE		"[cllp]terminate|%d[/cllp]"
#define CLLP_ISALIVE 		"[cllp]isalive|%d[/cllp]"
#define CLLP_DIED			"[cllp]died|%d[/cllp]"
#define CLLP_ERROR			"[cllp]error|%d[/cllp]"
#define CLLP_OK				"[cllp]ok|%d[/cllp]"
#define CLLP_CWD_OUT		"[cllp]cwd|%d|%s[/cllp]"
#define CLLP_CWD_IN			"[cllp]cwd|%d|%[^'['][/cllp]"
#define CLLP_INCLUDE1_IN	"[cllp]include1|%d|%s[/cllp]"
#define CLLP_INCLUDE1_OUT	"[cllp]include1|%d|%[^'['][/cllp]"
#define CLLP_INCLUDE2_IN	"[cllp]include2|%d|%s|%s[/cllp]"
#define CLLP_INCLUDE2_OUT	"[cllp]include2|%d|%[^'|']|%[^'['][/cllp]"

const std::string ClProLang::Hdr = "[cllp]";
const std::string ClProLang::Trl = "[/cllp]";

char* ClProLang::Fork(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_FORK);
	return ClLanguage::message->buffer;
}

char* ClProLang::Exec(const int pid, const std::string& call) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_EXEC_OUT, pid, call.c_str());
	return ClLanguage::message->buffer;
}

char* ClProLang::Terminate(const int pid) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_TERMINATE, pid);
	return ClLanguage::message->buffer;
}

char* ClProLang::IsAlive(const int pid) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_ISALIVE, pid);
	return ClLanguage::message->buffer;
}

char* ClProLang::Ok(const int pid) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_OK, pid);
	return ClLanguage::message->buffer;
}

char* ClProLang::Error(const int code) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_ERROR, code);
	return ClLanguage::message->buffer;
}

char* ClProLang::Died(const int pid) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_DIED, pid);
	return ClLanguage::message->buffer;
}

bool ClProLang::IsFork(const char* message) {
	return (strcmp(message, CLLP_FORK) == 0);
}

	bool ClProLang::IsExec(const char* message, int* pid, std::string* call) {
		if(sscanf(message, CLLP_EXEC_IN, pid, ClLanguage::_cache0->buffer) != 2)
			return false;
		call->assign(ClLanguage::_cache0->buffer);
		return true;
	}

bool ClProLang::IsIsAlive(const char* message, int* pid) {
	return(sscanf(message, CLLP_ISALIVE, pid) == 1);
}

bool ClProLang::IsTerminate(const char* message, int* pid) {
	return(sscanf(message, CLLP_TERMINATE, pid) == 1);
}

bool ClProLang::IsDied(const char* message, int* pid) {
	return(sscanf(message, CLLP_DIED, pid) == 1);
}

bool ClProLang::IsOk(const char* message, int* pid) {
	return(sscanf(message, CLLP_OK, pid) == 1);
}

bool ClProLang::IsError(const char* message, int* code) {
	return(sscanf(message, CLLP_ERROR, code) == 1);
}

char* ClProLang::Directory(const int pid, const std::string& path) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_CWD_OUT, pid, path.c_str());
	return ClLanguage::message->buffer;
}

char* ClProLang::Include(const int pid, const std::string& path){
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_INCLUDE1_IN, 
			pid, path.c_str());
	return ClLanguage::message->buffer;
}

char* ClProLang::Include(const int pid, const std::string& path0, 
		const std::string& path1){
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_INCLUDE2_IN, 
			pid, path0.c_str(), path1.c_str());
	return ClLanguage::message->buffer;
}

bool ClProLang::IsDirectory(const char* message, 
		int* pid, std::string* path) {
	int count = sscanf(message, CLLP_CWD_IN, 
			pid, 
			ClLanguage::_cache0->buffer);
	if(count < 2)
		return false;

	path->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClProLang::IsInclude(const char* message, int* pid, std::string* path) {
	int count = sscanf(message, CLLP_INCLUDE1_OUT, 
			pid, 
			ClLanguage::_cache0->buffer);
	if(count < 2)
		return false;

	path->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClProLang::IsInclude(const char* message, int* pid, 
		std::string* path0, std::string* path1) {

	int count = sscanf(message, CLLP_INCLUDE2_OUT, 
			pid, 
			ClLanguage::_cache0->buffer,  
			ClLanguage::_cache1->buffer);
	if(count < 3)
		return false;

	path0->assign(ClLanguage::_cache0->buffer);
	path1->assign(ClLanguage::_cache1->buffer);
	return true;
}

#endif
