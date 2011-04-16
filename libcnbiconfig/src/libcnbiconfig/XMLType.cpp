/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libxmlconfig library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "XMLType.hpp"
#include "XMLException.hpp"
#include <string.h>
#include <cstdio>
#include <iostream>

XMLType::XMLType(bool verbose) {
	this->_type = XMLType::TypeNone;
	this->_verbose = verbose;
	this->SetKey("unset");
	this->SetId("unset");
}
		
XMLType::XMLType(std::string buffer, bool verbose) {
	this->_type = XMLType::TypeNone;
	this->_verbose = verbose;
	this->Guess(buffer);
	this->SetKey("unset");
	this->SetId("unset");
}
		
XMLType::XMLType(const char* buffer, bool verbose) {
	this->_type = XMLType::TypeNone;
	this->_verbose = verbose;
	this->Guess(buffer);
	this->SetKey("unset");
	this->SetId("unset");
}

XMLType::~XMLType(void) {
}

void XMLType::SetKey(std::string key) {
	this->_key.assign(key);
}

void XMLType::SetKey(const char* key) {
	this->_key.assign(key);
}

std::string XMLType::Key(void) {
	return this->_key;
}

void XMLType::SetId(std::string id) {
	this->_id.assign(id);
}

void XMLType::SetId(const char* id) {
	this->_id.assign(id);
}

std::string XMLType::Id(void) {
	return this->_id;
}

XMLTypeData XMLType::Guess(std::string buffer) {
	return this->Guess((const char*)buffer.c_str());
}

XMLTypeData XMLType::Guess(const char* buffer) {
	if(this->_verbose)
		printf("[XMLType::Guess] Buffer: %s\n", buffer);

	char temp[1024];
	int isi = sscanf(buffer, "%d", &this->_vint);
	int isf = sscanf(buffer, "%f", &this->_vfloat);
	int iss = sscanf(buffer, "%s", temp);
	int isx = sscanf(buffer, "%x", &this->_vuint);

	if(this->_verbose)
		printf("  Relaxed matching for Int/Float/String/Hex: %d/%d/%d/%d\n", 
				isi, isf, iss, isx);

	if(isi && isf) {
		char* df = strchr((char*)buffer,  '.');
		char* dl = strrchr((char*)buffer, '.');
		char* hf = strrchr((char*)buffer, 'x');

		if(this->_verbose) 
			printf("  Strict numeric matching: "); 

		if(df != NULL && df == dl) {
			if(this->_verbose) 
				printf("Float\n"); 
			this->_type = XMLType::TypeFloat;
		}
		else if(df != NULL && df != dl) {
			if(this->_verbose) 
				printf("String\n"); 
			this->_type = XMLType::TypeString;
			this->_vstring.assign(temp);
		}
		else if(df == NULL && dl == NULL && hf == NULL) {
			if(this->_verbose)
				printf("Int\n"); 
			this->_type = XMLType::TypeInt;
		}
		else if(df == NULL && dl == NULL && hf != NULL) {
			if(this->_verbose) 
				printf("UInt\n"); 
			this->_type = XMLType::TypeUInt;
		}
	}
	else if(!isi && !isf) {
		if(this->_verbose) 
			printf("  Strict sting matching: "); 
		int itrue  = strcmp(buffer, "true");
		int ifalse = strcmp(buffer, "false");

		if(itrue != 0 && ifalse != 0) {
			if(this->_verbose)
				printf("String\n"); 

			this->_type = XMLType::TypeString;
			this->_vstring.assign(temp);
		}
		else if(itrue == 0 && ifalse != 0) {
			if(this->_verbose) 
				printf("Bool (true)\n"); 
	
			this->_type = XMLType::TypeBool;
			this->_vbool = true;
		}
		else if(itrue != 0 && ifalse == 0) {
			if(this->_verbose)
				printf("Bool (false)\n"); 
			
			this->_type = XMLType::TypeBool;
			this->_vbool = false;
		}
	}
	return this->_type;
}

bool XMLType::Bool(void) {
	if(this->_type != XMLType::TypeBool)
		throw XMLException("Type is not bool", "XMLType::Bool");
	return this->_vbool;
}

float XMLType::Float(void) {
	if(this->_type != XMLType::TypeFloat)
		throw XMLException("Type is not float", "XMLType::Float");
	return this->_vfloat;
}

int XMLType::Int(void) {
	if(this->_type != XMLType::TypeInt)
		throw XMLException("Type is not int", "XMLType::Int");
	return this->_vint;
}

unsigned int XMLType::UInt(void) {
	if(this->_type != XMLType::TypeUInt)
		throw XMLException("Type is not uint", "XMLType::UInt");
	return this->_vuint;
}

std::string XMLType::String(void) {
	if(this->_type != XMLType::TypeString)
		throw XMLException("Type is not string", "XMLType::String");
	return this->_vstring;
}
