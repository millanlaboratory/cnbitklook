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

#include "XMLDocument.hpp"
#include "XMLException.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
		
XMLDocument::XMLDocument(unsigned int bsize) {
	this->_bsize = bsize;
	this->_buffer = new char[this->_bsize];
	this->FillBuffer(NULL);
}

XMLDocument::~XMLDocument() {
}

void XMLDocument::ImportFile(std::string filename) {
	std::ifstream file;
	std::string cache, buffer;
	file.open(filename.c_str(), std::ios::in);
	
	if(file == NULL) {
		std::string info;
		info += "XML file not found: ";
		info += filename;
		throw XMLException(info, __PRETTY_FUNCTION__);
	}

	while(!file.eof()) {
		getline(file, cache);
		buffer.append(cache);
		buffer.append("\n");
	}
	file.close();
	
	this->FillBuffer(&buffer);
	this->Parse();
}

void XMLDocument::ImportBuffer(std::string buffer) {
	//TODO
}

void XMLDocument::Dump(void) {
	printf("[XMLDocument::Dump] Dumping internal buffer [%d/%d bytes]\n", 
			(int)strlen(this->_buffer), this->_bsize);
	printf("%s\n", this->_buffer);
}

void XMLDocument::Stats(void) {
	printf("[XMLDocument::Stats] Stats:\n");
	printf("  Buffer size: %d bytes\n", this->_bsize);
	printf("  Buffer used: %d bytes\n", (int)strlen(this->_buffer));
}
	
void XMLDocument::Parse(void) {
	try {
		this->doc.parse<0>(this->_buffer);
	} catch(rapidxml::parse_error e) {
		// TODO: use rapidxml::parse_error::where()
		throw XMLException("XML is malformed", __PRETTY_FUNCTION__);
	}
}

void XMLDocument::FillBuffer(std::string* buffer) {
	memset(this->_buffer, 0, this->_bsize); 

	if(buffer == NULL)
		return;

	if(buffer->size() > this->_bsize)
		throw XMLException("Buffer too small to load XML", __PRETTY_FUNCTION__);
	memcpy(this->_buffer, buffer->c_str(), buffer->size());
}
