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

#include "CcBasic.hpp"
#include "CcSemaphore.hpp"
#include <transpipe/tp_namedpipe.h>
#include <string>

/*! \brief Named pipe 
 *
 * Simple wrapper for libtranspipe. Raises exceptions.
 */
class CcPipe : public CcObject {
	public:
		CcPipe(void);
		~CcPipe(void);
		bool Open(const std::string& filename, int mode = CcPipe::Reader);
		bool Close(void);
		ssize_t Write(const void* buffer, const size_t bsize);
		ssize_t TryWrite(const void* buffer, const size_t bsize);
		ssize_t Read(void* buffer, const size_t bsize);
		ssize_t TryRead(void* buffer, const size_t bsize);
		static void CatchSIGPIPE(void);
		bool IsBroken(void);
	private:
		bool OpenRead(void);
		bool OpenWrite(void);

	public:
		static const int Unset = -1;
		static const int Reader = 0;
		static const int Writer = 1;
	protected:
		int _mode;
		tp_npipe _pipe;
		CcSemaphore _sempipe;
};

#endif
