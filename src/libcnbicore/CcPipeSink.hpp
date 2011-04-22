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

#ifndef CCPIPESINK_HPP 
#define CCPIPESINK_HPP 

#include "CcPipe.hpp"

/*! \brief Named pipe receiver
 *
 * Simple wrapper for libtranspipe, forcing users to catch exceptions
 * It allows to read from multiple threads.
 */
class CcPipeSink : public CcPipe {
	public:
		CcPipeSink(const std::string& filename);
		virtual ~CcPipeSink(void);
		virtual void Open(void);
		virtual void Open(double wait);
		virtual void Close(void);
		virtual size_t Read(void* buffer, const size_t bsize);
		virtual bool Read(void* buffer, const size_t bsize, size_t* rsize);
};

#endif
