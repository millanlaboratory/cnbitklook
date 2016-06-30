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
		void Clear(void);
		void Write(const T* buffer, size_t size = 0);
		void Read(T* buffer, size_t size = 0);
		void Write(const CcBuffer* buffer, size_t size = 0);
		void Read(CcBuffer* buffer, size_t size = 0);
	public:
		T* buffer;
		size_t bsize;
};

#include "CcBuffer.cpp" 

#endif
