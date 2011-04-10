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

#ifndef CLPROLANG_CPP 
#define CLPROLANG_CPP 

#include "ClProLang.hpp" 
#include <stdio.h>

#define CLLP_FORK					"[cllp]fork[/cllp]"
#define CLLP_LAUNCH_MATLAB_OUT		"[cllp]launch_matlab|%d|%s[/cllp]"
#define CLLP_LAUNCH_MATLAB_IN		"[cllp]launch_matlab|%d|%[^'['][/cllp]"
#define CLLP_TERMINATE				"[cllp]terminate|%d[/cllp]"
#define CLLP_ISALIVE 				"[cllp]isalive|%d[/cllp]"
#define CLLP_DIED					"[cllp]died|%d[/cllp]"
#define CLLP_ERROR					"[cllp]error|%d[/cllp]"
#define CLLP_OK						"[cllp]ok|%d[/cllp]"
#define CLLP_CWD_OUT				"[cllp]cwd|%d|%s[/cllp]"
#define CLLP_CWD_IN					"[cllp]cwd|%d|%[^'['][/cllp]"
#define CLLP_INCLUDE_IN				"[cllp]include|%d|%s|%s[/cllp]"
#define CLLP_INCLUDE_OUT			"[cllp]include|%d|%[^'|']|%[^'['][/cllp]"
#define CLLP_LAUNCH_NDFMATLAB_OUT 	"[cllp]launch_ndfmatlab|%d|%s|%s|%s|%s|%s[/cllp]"
#define CLLP_LAUNCH_NDFMATLAB_IN  	"[cllp]launch_ndfmatlab|%d|%[^'|']|%[^'|']|%[^'|']|%[^'|']|%[^'['][/cllp]"

const std::string ClProLang::Hdr = "[cllp]";
const std::string ClProLang::Trl = "[/cllp]";

char* ClProLang::Fork(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_FORK);
	return ClLanguage::message->buffer;
}

char* ClProLang::Launch(const int pid, const std::string& call) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_LAUNCH_MATLAB_OUT, pid, call.c_str());
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

	bool ClProLang::IsLaunch(const char* message, int* pid, std::string* call) {
		if(sscanf(message, CLLP_LAUNCH_MATLAB_IN, pid, ClLanguage::_cache0->buffer) != 2)
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

char* ClProLang::ChangeDirectory(const int pid, const std::string& path) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_CWD_OUT, pid, path.c_str());
	return ClLanguage::message->buffer;
}

char* ClProLang::Include(const int pid, const std::string& path0, 
		const std::string& path1){
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_INCLUDE_IN, 
			pid, path0.c_str(), path1.c_str());
	return ClLanguage::message->buffer;
}

char* ClProLang::LaunchNDF(int pid,
		const std::string& function, const std::string& pipename,
		const CcAddress addressD, const CcAddress addressC, 
		const std::string& extra) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLP_LAUNCH_NDFMATLAB_OUT, 
			pid,
			function.c_str(), 
			pipename.c_str(),
			addressD.c_str(), 
			addressC.c_str(), 
			extra.c_str());
	return ClLanguage::message->buffer;
}

bool ClProLang::IsChangeDirectory(const char* message, 
		int* pid, std::string* path) {
	int count = sscanf(message, CLLP_CWD_IN, 
			pid, 
			ClLanguage::_cache0->buffer);
	if(count < 2)
		return false;

	path->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClProLang::IsInclude(const char* message, int* pid, 
		std::string* path0, std::string* path1) {

	int count = sscanf(message, CLLP_INCLUDE_OUT, 
			pid, 
			ClLanguage::_cache0->buffer,  
			ClLanguage::_cache1->buffer);
	if(count < 3)
		return false;

	path0->assign(ClLanguage::_cache0->buffer);
	path1->assign(ClLanguage::_cache1->buffer);
	return true;

}

bool ClProLang::IsLaunchNDF(const char* message, 
		int* pid,
		std::string* function, std::string* pipename, 
		std::string* addressD, std::string* addressC, 
		std::string* extra) {

	int count = sscanf(message, CLLP_LAUNCH_NDFMATLAB_IN, 
			pid,
			ClLanguage::_cache0->buffer,  
			ClLanguage::_cache1->buffer,  
			ClLanguage::_cache2->buffer,  
			ClLanguage::_cache3->buffer,  
			ClLanguage::_cache4->buffer);
	if(count < 6-1)
		return false;

	function->assign(ClLanguage::_cache0->buffer);
	pipename->assign(ClLanguage::_cache1->buffer);
	addressD->assign(ClLanguage::_cache2->buffer);
	addressC->assign(ClLanguage::_cache3->buffer);
	extra->assign(ClLanguage::_cache4->buffer);

	return true;
}

#endif
