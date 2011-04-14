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

#ifndef CCPIPEWRITER_HPP 
#define CCPIPEWRITER_HPP 

#include "CcBasic.hpp"
#include "CcDoubleBuffer.hpp"
#include "CcPipeSource.hpp"
#include "CcSemaphore.hpp"
#include "CcThread.hpp"
#include "CcThreadSafe.hpp"
#include <string.h>

/* Forward declaration */
class CcPipeWriterProxy;

/*! \brief Threaded pipe source that supports ACK
 *
 * Used to send point-to-point data configuring the enpoint
 */
class CcPipeWriter : public CcThread {
	public:
		CcPipeWriter(size_t bsize = 0);
		virtual ~CcPipeWriter(void);
		virtual void Open(const std::string& filename);
		virtual void Open(void);
		virtual void Close(void);
		virtual bool IsBroken(void);
		virtual bool IsOpen(void);
		virtual size_t Write(const void* buffer, const size_t bsize);
		virtual size_t Write(const char* buffer);
		virtual size_t Write(std::string& buffer);
		virtual void BWrite(const void* buffer);
		virtual std::string GetFilename(void);
		virtual void Acknoledge(const void* buffer, const size_t bsize);
	private:
	protected:
		virtual void Main(void);

	public:
		CcCallback1<CcPipeWriterProxy, CcPipeWriter*> iOnOpen;
		CcCallback1<CcPipeWriterProxy, CcPipeWriter*> iOnClose;
		CcCallback1<CcPipeWriterProxy, CcPipeWriter*> iOnBroken;
		CcCallback1<CcPipeWriterProxy, CcPipeWriter*> iOnWrite;
	private:
	protected:
		CcPipeSource* _pipe;
		CcThreadSafe<> _isopen;
		std::string _filename;
		CcThreadSafe<> _bufferedmode;
		CcDoubleBuffer* _wbuff;
		void* _ackbuffer;
		size_t _ackbsize;
};

class CcPipeWriterProxy {
	public:
		virtual void HandleOpen(CcPipeWriter* caller) { }
		virtual void HandleClose(CcPipeWriter* caller) { }
		virtual void HandleBroken(CcPipeWriter* caller) { }
		virtual void HandleWrite(CcPipeWriter* caller) { }
};

#define CB_CcPipeWriter(sender_event, receiver_ptr, method) 		\
	sender_event.Register((CcPipeWriterProxy*)receiver_ptr, 		\
			&CcPipeWriterProxy::method);

#endif
