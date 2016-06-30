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

#ifndef CCBUFFER_CPP 
#define CCBUFFER_CPP 

#include "CcBuffer.hpp" 
#include "CcBasic.hpp" 
#include <stdlib.h>
#include <string.h>

template <class T>
CcBuffer<T>::CcBuffer(size_t size) {
	this->bsize = size;
	this->buffer = (T*)malloc(this->bsize);
	this->Clear();
}

template <class T>
CcBuffer<T>::~CcBuffer(void) {
	free(this->buffer);
}
		
template <class T>
void CcBuffer<T>::Clear(void) {
	memset(this->buffer, 0, this->bsize);
}
		
template <class T>
void CcBuffer<T>::Write(const T* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Write would overflow");
	
	if(size == 0)
		memcpy(this->buffer, buffer, this->bsize);
	else
		memcpy(this->buffer, buffer, size);
}

template <class T>
void CcBuffer<T>::Read(T* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Read would overflow");

	if(size == 0)
		memcpy(buffer, this->buffer, this->bsize);
	else
		memcpy(buffer, this->buffer, size);
}

template <class T>
void CcBuffer<T>::Write(const CcBuffer* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Write would overflow");
	
	if(size == 0)
		memcpy(this->buffer, buffer->buffer, this->bsize);
	else
		memcpy(this->buffer, buffer->buffer, size);
}

template <class T>
void CcBuffer<T>::Read(CcBuffer* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Read would overflow");

	if(size == 0)
		memcpy(buffer->buffer, this->buffer, this->bsize);
	else
		memcpy(buffer->buffer, this->buffer, size);
}

#endif
