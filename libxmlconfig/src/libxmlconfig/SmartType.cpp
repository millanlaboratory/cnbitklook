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

#include "SmartType.hpp"
#include <string.h>
#include <cstdio>
#include <iostream>
#include "XMLException.hpp"

SmartType::SmartType(bool verbose) {
	this->_type = SmartType::TypeNone;
	this->_verbose = verbose;
	this->SetKey("unset");
	this->SetId("unset");
}
		
SmartType::SmartType(std::string buffer, bool verbose) {
	this->_type = SmartType::TypeNone;
	this->_verbose = verbose;
	this->Guess(buffer);
	this->SetKey("unset");
	this->SetId("unset");
}
		
SmartType::SmartType(const char* buffer, bool verbose) {
	this->_type = SmartType::TypeNone;
	this->_verbose = verbose;
	this->Guess(buffer);
	this->SetKey("unset");
	this->SetId("unset");
}

SmartType::~SmartType(void) {
}

void SmartType::SetKey(std::string key) {
	this->_key.assign(key);
}

void SmartType::SetKey(const char* key) {
	this->_key.assign(key);
}

std::string SmartType::Key(void) {
	return this->_key;
}

void SmartType::SetId(std::string id) {
	this->_id.assign(id);
}

void SmartType::SetId(const char* id) {
	this->_id.assign(id);
}

std::string SmartType::Id(void) {
	return this->_id;
}

SmartData SmartType::Guess(std::string buffer) {
	return this->Guess((const char*)buffer.c_str());
}

SmartData SmartType::Guess(const char* buffer) {
	if(this->_verbose)
		printf("[SmartType::Guess] Buffer: %s\n", buffer);

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
			this->_type = SmartType::TypeFloat;
		}
		else if(df != NULL && df != dl) {
			if(this->_verbose) 
				printf("String\n"); 
			this->_type = SmartType::TypeString;
			this->_vstring.assign(temp);
		}
		else if(df == NULL && dl == NULL && hf == NULL) {
			if(this->_verbose)
				printf("Int\n"); 
			this->_type = SmartType::TypeInt;
		}
		else if(df == NULL && dl == NULL && hf != NULL) {
			if(this->_verbose) 
				printf("UInt\n"); 
			this->_type = SmartType::TypeUInt;
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

			this->_type = SmartType::TypeString;
			this->_vstring.assign(temp);
		}
		else if(itrue == 0 && ifalse != 0) {
			if(this->_verbose) 
				printf("Bool (true)\n"); 
	
			this->_type = SmartType::TypeBool;
			this->_vbool = true;
		}
		else if(itrue != 0 && ifalse == 0) {
			if(this->_verbose)
				printf("Bool (false)\n"); 
			
			this->_type = SmartType::TypeBool;
			this->_vbool = false;
		}
	}
	return this->_type;
}

bool SmartType::Bool(void) {
	if(this->_type != SmartType::TypeBool)
		throw XMLException("Type is not bool", "SmartType::Bool");
	return this->_vbool;
}

float SmartType::Float(void) {
	if(this->_type != SmartType::TypeFloat)
		throw XMLException("Type is not float", "SmartType::Float");
	return this->_vfloat;
}

int SmartType::Int(void) {
	if(this->_type != SmartType::TypeInt)
		throw XMLException("Type is not int", "SmartType::Int");
	return this->_vint;
}

unsigned int SmartType::UInt(void) {
	if(this->_type != SmartType::TypeUInt)
		throw XMLException("Type is not uint", "SmartType::UInt");
	return this->_vuint;
}

std::string SmartType::String(void) {
	if(this->_type != SmartType::TypeString)
		throw XMLException("Type is not string", "SmartType::String");
	return this->_vstring;
}
