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

#ifndef CLLANGUAGE_CPP 
#define CLLANGUAGE_CPP 

#include "ClLanguage.hpp" 

ClLanguage::ClLanguage(size_t msize, size_t csize) {
	this->message = new CcBuffer<char>(msize);
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

size_t ClLanguage::MessageSize(void) {
	return this->message->bsize;
}

size_t ClLanguage::CacheSize(void) {
	return this->_cache0->bsize;
}

#endif
