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

#ifndef CCDOUBLEBUFFER_HPP 
#define CCDOUBLEBUFFER_HPP 

#include "CcBasic.hpp"
#include "CcSemaphore.hpp"
#include "CcBuffer.hpp"

/*! \brief Double buffer 
 *
 * This class implements a simple double-buffering controller that switches
 * between a reading and a writing buffer. The class uses a void pointer to
 * represent the internal buffer. The internal buffers are of CcBuffer type.
 *
 * This class becomes particularly handy to manage asynchronous read/writes.
 */
class CcDoubleBuffer : public CcObject {
	public:
		CcDoubleBuffer(size_t bsize);
		virtual ~CcDoubleBuffer(void);
		void Write(const void* buffer);
		void Read(void* buffer);
		void Prepare(void);
		size_t Bsize(void);
	private:
		void Swap(void);

	private:
		CcBuffer<>* _buffer0;
		CcBuffer<>* _buffer1;
		CcBuffer<>* _bufferr;
		CcBuffer<>* _bufferw;
		CcSemaphore _semlock;
		CcSemaphore _semread;
		size_t _bsize;
};

#endif
