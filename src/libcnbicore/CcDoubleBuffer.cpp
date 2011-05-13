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

#ifndef CCDOUBLEBUFFER_CPP 
#define CCDOUBLEBUFFER_CPP 

#include "CcDoubleBuffer.hpp" 
#include <stdlib.h>
#include <string.h>

CcDoubleBuffer::CcDoubleBuffer(size_t bsize) {
	this->_bsize = bsize;
	this->_buffer0 = new CcBuffer<>(bsize);
	this->_buffer1 = new CcBuffer<>(bsize);
	this->_bufferr = this->_buffer0;
	this->_bufferw = this->_buffer1;
	this->Prepare();
}

CcDoubleBuffer::~CcDoubleBuffer(void) {
	free(this->_buffer0);
	free(this->_buffer1);
}

void CcDoubleBuffer::Write(const void* buffer) {
	this->_semlock.Post();
	this->_bufferw->Write(buffer);
	this->_semlock.Post();
	this->_semread.Post();
}

void CcDoubleBuffer::Read(void* buffer) {
	this->_semread.Wait();
	this->_semlock.Wait();
	this->_bufferr->Read(buffer);
	this->Swap();
	this->_semlock.Post();
}

void CcDoubleBuffer::Swap(void) {
	if(this->_bufferr == this->_buffer0) {
		this->_bufferr = this->_buffer1;
		this->_bufferw = this->_buffer0;
	} else {
		this->_bufferr = this->_buffer0;
		this->_bufferw = this->_buffer1;
	}
}
		
size_t CcDoubleBuffer::Bsize(void) {
	return this->_bsize;
}

void CcDoubleBuffer::Prepare(void) {
	this->_semlock.Wait();
	this->_buffer0->Clear();
	this->_buffer1->Clear();
	this->_semlock.Post();

	this->_semread.TryWait();
}

#endif
