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

#ifndef CCPIPESOURCE_HPP 
#define CCPIPESOURCE_HPP 

#include "CcPipe.hpp"

/*! \brief Named pipe sender
 *
 * Simple wrapper for libtranspipe, forcing users to catch exceptions.
 * It allows to write from multiple threads.
 */
class CcPipeSource : public CcPipe {
	public:
		CcPipeSource(const std::string& filename);
		virtual ~CcPipeSource(void);
		virtual void Open(void);
		virtual void Close(void);
		virtual size_t Write(const void* buffer, const size_t bsize);
		virtual bool Write(const void* buffer, const size_t bsize, 
				size_t* wsize);
		virtual bool IsBroken(void);
		static void CatchSIGPIPE(void);
	private:
		virtual void Clean(void);
};

#endif
