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

#ifndef CLNMSLANG_CPP 
#define CLNMSLANG_CPP 

#include "ClNmsLang.hpp" 
#include <cnbicore/CcFlags.hpp>
#include <stdio.h>
#include <stdexcept>

#define CLLN_ERROR		"[clln]error|%d[/clln]"
#define CLLN_OK			"[clln]ok[/clln]"
#define CLLN_QUERY_IN	"[clln]query|%[^'['][/clln]"
#define CLLN_QUERY_OUT	"[clln]query|%s[/clln]"
#define CLLN_REPLY_IN	"[clln]reply|%[^'['][/clln]"
#define CLLN_REPLY_OUT	"[clln]reply|%s[/clln]"
#define CLLN_SET_IN		"[clln]set|%[^'|']|%[^'['][/clln]"
#define CLLN_SET_OUT	"[clln]set|%s|%s[/clln]"
#define CLLN_UNSET_IN	"[clln]unset|%[^'['][/clln]"
#define CLLN_UNSET_OUT	"[clln]unset|%s[/clln]"

#define CLLN_DISP_IN	"[clln]dispatch|%lu|%*s[/clln]"
#define CLLN_DISP_OUT	"[clln]dispatch|%lu|%s[/clln]"
#define CLLN_RETR_IN	"[clln]retrieve|%[^'['][/clln]"
#define CLLN_RETR_OUT	"[clln]retrieve|%s[/clln]"
#define CLLN_STOR_IN	"[clln]store|%[^'|']|%lu|%*s[/clln]"
#define CLLN_STOR_OUT	"[clln]store|%s|%lu|%s[/clln]"
#define CLLN_ERASE_IN	"[clln]erase|%[^'['][/clln]"
#define CLLN_ERASE_OUT	"[clln]erase|%s[/clln]"

const std::string ClNmsLang::Hdr = "[clln]";
const std::string ClNmsLang::Trl = "[/clln]";

ClNmsLang::ClNmsLang(void) : ClLanguage(CCCORE_1MB) {
}
		
char* ClNmsLang::Query(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(), 
			CLLN_QUERY_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Reply(CcAddress address) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_REPLY_OUT, address.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Set(const std::string& name, CcAddress address) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_SET_OUT, name.c_str(), address.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Unset(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_UNSET_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Store(const std::string& name, const std::string& content) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_STOR_OUT, name.c_str(), content.size(), content.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Retrieve(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_RETR_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Dispatch(const std::string& content) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_DISP_OUT, content.size(), content.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Erase(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_ERASE_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Ok(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_OK);
	return ClLanguage::message->buffer;
}

char* ClNmsLang::Error(const int code) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_ERROR, code);
	return ClLanguage::message->buffer;
}

bool ClNmsLang::IsQuery(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_QUERY_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNmsLang::IsReply(const char* message, CcAddress* address) {
	int count = sscanf(message, CLLN_REPLY_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	address->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNmsLang::IsSet(const char* message, std::string* name, 
		CcAddress* address) {
	int count = sscanf(message, CLLN_SET_IN, ClLanguage::_cache0->buffer, 
			ClLanguage::_cache1->buffer);
	if(count < 2)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	address->assign(ClLanguage::_cache1->buffer);
	return true;
}

bool ClNmsLang::IsUnset(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_UNSET_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNmsLang::IsStore(const char* message, std::string* name, 
		std::string* content) {
	size_t size = 0;
	int count = sscanf(message, CLLN_STOR_IN, ClLanguage::_cache0->buffer,
			&size);
	if(count < 2)
		return false;
	
	name->assign(ClLanguage::_cache0->buffer);
	
	std::string cache(message);
	size_t trl = cache.find(ClNmsLang::Trl);
	if(trl == std::string::npos)
		return false;

	try {
		content->assign(cache.substr(trl-size, size));
	} catch(std::out_of_range& e) {
		CcLogFatal("Out of range exception")
		return false;
	}

	return true;
}

bool ClNmsLang::IsRetrieve(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_RETR_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNmsLang::IsErase(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_ERASE_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNmsLang::IsDispatch(const char* message, std::string* content) {
	size_t size = 0;
	int count = sscanf(message, CLLN_DISP_IN, &size);
	if(count < 1)
		return false;

	std::string cache(message);
	size_t trl = cache.find(ClNmsLang::Trl);
	if(trl == std::string::npos)
		return false;
	
	try {
		content->assign(cache.substr(trl-size, size));
	} catch(std::out_of_range& e) {
		CcLogFatal("Out of range exception")
		return false;
	}

	return true;
}

bool ClNmsLang::IsOk(const char* message) {
	return(strcmp(CLLN_OK, message) == 0);
}

bool ClNmsLang::IsError(const char* message, int* code) {
	return(sscanf(message, CLLN_ERROR, code) == 1);
}
		
bool ClNmsLang::CheckName(const char* message) {
	char name[128];
	return(sscanf(message, "/%s", name) == 1);
}

#endif
