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
		
XMLDocument::XMLDocument(size_t bsize) {
	this->_bsize = bsize;
	this->_buffer = new char[this->_bsize];
}

XMLDocument::~XMLDocument() {
	delete(this->_buffer);
}

int XMLDocument::ImportFile(const std::string& filename) {
	std::ifstream file;
	std::string cache, buffer;
	file.open(filename.c_str(), std::ios::in);
	
	if(file == NULL) 
		return XMLDocument::FileInputError;

	while(!file.eof()) {
		getline(file, cache);
		buffer.append(cache);
		buffer.append("\n");
	}
	file.close();
	
	int status = this->FillBuffer(&buffer);
	if(status != XMLDocument::Successfull)
		return status;
	return this->Parse();
}

int XMLDocument::ImportBuffer(const std::string& buffer) {
	int status = this->FillBuffer(&buffer);
	if(status != XMLDocument::Successfull)
		return status;
	return this->Parse();
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

int XMLDocument::FillBuffer(const std::string* buffer) {
	memset(this->_buffer, 0, this->_bsize); 

	if(buffer->size() > this->_bsize)
		return XMLDocument::BufferOverflow;
	memcpy(this->_buffer, buffer->c_str(), buffer->size());
	return XMLDocument::Successfull;
}
	
int XMLDocument::Parse(void) {
	try {
		this->doc.parse<0>(this->_buffer);
	} catch(rapidxml::parse_error e) {
		return XMLDocument::ParsingError;
	}
	return XMLDocument::Successfull;
}
