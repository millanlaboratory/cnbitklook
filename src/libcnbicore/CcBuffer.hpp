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

#ifndef CCBUFFER_HPP 
#define CCBUFFER_HPP 

#include "CcBasic.hpp"

/*! \brief Buffer template class
 *
 * The template class implements a simple buffer that can be used for buffering
 * data from the network or, for example, from an EEG device
 */
template <class T=void>
class CcBuffer : public CcObject {
	public:
		CcBuffer(size_t size);
		virtual ~CcBuffer(void);
		virtual void Clear(void);
		virtual void Write(const T* buffer, size_t size = 0);
		virtual void Read(T* buffer, size_t size = 0);
		virtual void Write(const CcBuffer* buffer, size_t size = 0);
		virtual void Read(CcBuffer* buffer, size_t size = 0);
	public:
		T* buffer;
		size_t bsize;
};

#include "CcBuffer.cpp" 

#endif
