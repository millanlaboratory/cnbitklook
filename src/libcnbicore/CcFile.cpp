/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

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

#ifndef CCFILE_CPP 
#define CCFILE_CPP 

#include "CcFile.hpp" 
#include <fstream>
#include <iterator>

CcFile::CcFile(void) {
}

CcFile::~CcFile(void) {
}

bool CcFile::Load(const std::string& filename) {
	this->_semdata.Wait();
	std::ifstream stream(filename.c_str(), std::ifstream::in);
	if(stream.good() == false) {
		this->_semdata.Post();
		return false;
	}
	
	this->_data.clear();
	std::stringstream buffer;
	buffer << stream.rdbuf();
	this->_data.assign(buffer.str());

	stream.close();
	this->_semdata.Post();
	return true;
}

bool CcFile::Save(const std::string& filename) {
	this->_semdata.Wait();
	std::ofstream stream(filename.c_str());
	if(stream.good() == false) {
		this->_semdata.Post();
		return false;
	}
	
	stream << this->_data;
	stream.close();
	this->_semdata.Post();
	return true;
}

void CcFile::Set(const std::string& data) {
	this->_semdata.Wait();
	this->_data.assign(data);
	this->_semdata.Post();
}

void CcFile::Get(std::string* data) {
	this->_semdata.Wait();
	data->assign(this->_data);
	this->_semdata.Post();
}

std::string CcFile::Get(void) {
	std::string data;
	this->_semdata.Wait();
	data.assign(this->_data);
	this->_semdata.Post();
	return data;
}

void CcFile::Clear(void) {
	this->_semdata.Wait();
	this->_data.clear();
	this->_semdata.Post();
}
		
size_t CcFile::Size(void) {
	size_t size;
	this->_semdata.Wait();
	size = this->_data.size();
	this->_semdata.Post();
	return size;
}
		
size_t CcFile::MaxSize(void) {
	size_t size;
	this->_semdata.Wait();
	size = this->_data.max_size();
	this->_semdata.Post();
	return size;
}

#endif
