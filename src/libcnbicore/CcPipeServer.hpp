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

#ifndef CCPIPESERVER_HPP 
#define CCPIPESERVER_HPP 

#include "CcPipeWriter.hpp"
#include <vector>

/*! \brief Asynchronous pipe writer 
 * 
 * Used to multicast data configuring the enpoints. As soon ad a connection is
 * estabilidshe
 */
class CcPipeServer : public CcObject {
	public:
		CcPipeServer(void* ackbuffer = NULL, size_t ackbsize = 0,
				size_t bsize = 0, bool reopen = true);
		virtual ~CcPipeServer();
		virtual void Open(const std::string& filename, unsigned int total);
		virtual void Close(void);
		virtual void Write(const void* buffer, size_t bsize = 0);
	private:
	protected:

	public:
	private:
	protected:
		std::vector<CcPipeWriter*> _pipes;
		CcThreadSafe<> _reopen;
		CcThreadSafe<> _isopen;
		void* _ackbuffer;
		size_t _ackbsize;
		size_t _bsize;
};

#endif
