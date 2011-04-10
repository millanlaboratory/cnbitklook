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

#ifndef CLNAMESLANG_CPP 
#define CLNAMESLANG_CPP 

#include "ClNamesLang.hpp" 
#include <stdio.h>

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

#define CLLN_DISP_IN	"[clln]dispatch|%[^'['][/clln]"
#define CLLN_DISP_OUT	"[clln]dispatch|%s[/clln]"
#define CLLN_RETR_IN	"[clln]retrieve|%[^'['][/clln]"
#define CLLN_RETR_OUT	"[clln]retrieve|%s[/clln]"
#define CLLN_STOR_IN	"[clln]store|%[^'|']|%lu|%[^'['][/clln]"
#define CLLN_STOR_OUT	"[clln]store|%s|%lu|%s[/clln]"
#define CLLN_ERASE_IN	"[clln]erase|%[^'['][/clln]"
#define CLLN_ERASE_OUT	"[clln]erase|%s[/clln]"

const std::string ClNamesLang::Hdr = "[clln]";
const std::string ClNamesLang::Trl = "[/clln]";
		
ClNamesLang::ClNamesLang(void) : ClLanguage(2200000, 2048, 2*1048576) {
}
		
char* ClNamesLang::Query(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(), 
			CLLN_QUERY_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Reply(CcAddress address) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_REPLY_OUT, address.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Set(const std::string& name, CcAddress address) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_SET_OUT, name.c_str(), address.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Unset(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_UNSET_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Store(const std::string& name, const std::string& content) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_STOR_OUT, name.c_str(), content.size(), content.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Retrieve(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_RETR_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Dispatch(const std::string& content) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_DISP_OUT, content.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Erase(const std::string& name) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_ERASE_OUT, name.c_str());
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Ok(void) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_OK);
	return ClLanguage::message->buffer;
}

char* ClNamesLang::Error(const int code) {
	snprintf(ClLanguage::message->buffer, ClLanguage::MessageSize(),
			CLLN_ERROR, code);
	return ClLanguage::message->buffer;
}

bool ClNamesLang::IsQuery(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_QUERY_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNamesLang::IsReply(const char* message, CcAddress* address) {
	int count = sscanf(message, CLLN_REPLY_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	address->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNamesLang::IsSet(const char* message, std::string* name, 
		CcAddress* address) {
	int count = sscanf(message, CLLN_SET_IN, ClLanguage::_cache0->buffer, 
			ClLanguage::_cache1->buffer);
	if(count < 2)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	address->assign(ClLanguage::_cache1->buffer);
	return true;
}

bool ClNamesLang::IsUnset(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_UNSET_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNamesLang::IsStore(const char* message, std::string* name, 
		std::string* content) {
	size_t size;
	int count = sscanf(message, CLLN_STOR_IN, ClLanguage::_cache0->buffer,
			&size, ClLanguage::_cache1->buffer);
	if(count < 3)
		return false;
	
	name->assign(ClLanguage::_cache0->buffer);
	content->assign(ClLanguage::_cache1->buffer);
	
	/*
	std::string cache(message);
	std::cout << cache << std::endl;
	size_t trl = cache.find(ClNamesLang::Trl);
	if(trl == std::string::npos)
		return false;
	content->assign(cache.substr(trl-size, size));
	*/
	return true;
}

bool ClNamesLang::IsRetrieve(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_RETR_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNamesLang::IsErase(const char* message, std::string* name) {
	int count = sscanf(message, CLLN_ERASE_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	name->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNamesLang::IsDispatch(const char* message, std::string* content) {
	int count = sscanf(message, CLLN_DISP_IN, ClLanguage::_cache0->buffer);
	if(count < 1)
		return false;

	content->assign(ClLanguage::_cache0->buffer);
	return true;
}

bool ClNamesLang::IsOk(const char* message) {
	return(strcmp(CLLN_OK, message) == 0);
}

bool ClNamesLang::IsError(const char* message, int* code) {
	return(sscanf(message, CLLN_ERROR, code) == 1);
}

#endif
