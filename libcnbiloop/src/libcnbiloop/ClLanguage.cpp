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

#ifndef CLLANGUAGE_CPP 
#define CLLANGUAGE_CPP 

#include "ClLanguage.hpp" 

ClLanguage::ClLanguage(size_t bsize, size_t csize) {
	this->_bsize = bsize;
	this->message = new CcBuffer<char>(this->_bsize);
	this->_cache0 = new CcBuffer<char>(csize);
	this->_cache1 = new CcBuffer<char>(csize);
	this->_cache2 = new CcBuffer<char>(csize);
	this->_cache3 = new CcBuffer<char>(csize);
	this->_cache4 = new CcBuffer<char>(csize);
	this->_cache5 = new CcBuffer<char>(csize);
}

ClLanguage::~ClLanguage(void) {
	delete this->message;
	delete this->_cache0;
	delete this->_cache1;
	delete this->_cache2;
	delete this->_cache3;
	delete this->_cache4;
	delete this->_cache5;
}

size_t ClLanguage::MaxSize(void) {
	return this->_bsize;
}

#endif
