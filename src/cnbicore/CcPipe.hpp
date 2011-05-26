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

#ifndef CCPIPE_HPP 
#define CCPIPE_HPP 

#include "CcObject.hpp"
#include "CcSemaphore.hpp"
#include <transpipe/tp_namedpipe.h>
#include <string>

/*! \brief Named pipe 
 *
 * Simple wrapper for libtranspipe. Raises exceptions.
 */
class CcPipe : public CcObject {
	public:
		CcPipe(const std::string& filename);
		virtual ~CcPipe(void);
		virtual void Open(void) = 0;
		virtual void Close(void) = 0;
		virtual std::string GetFilename(void);
	private:
		virtual void Clean(void);
		
	protected:
		tp_npipe _pipe;
		CcSemaphore _sempipe;
};

#endif
