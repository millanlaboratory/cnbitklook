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

#ifndef CCPIPEREADER_HPP 
#define CCPIPEREADER_HPP 

#include "CcBasic.hpp"
#include "CcBuffer.hpp"
#include "CcPipeSink.hpp"
#include "CcSemaphore.hpp"
#include "CcStreamer.hpp"
#include "CcThread.hpp"
#include "CcThreadSafe.hpp"

/* Forward declaration */
class CcPipeReaderProxy;

/*! \brief Threaded pipe sink 
 *
 * Used to receive point-to-point data
 */
class CcPipeReader : public CcThread {
	public:
		CcPipeReader(size_t bsize = 4096);
		virtual ~CcPipeReader(void);
		virtual void Open(const std::string& filename);
		virtual void Close(void);
		virtual bool IsBroken(void);
		virtual bool IsOpen(void);
	private:
	protected:
		virtual void Main(void);

	public:
		CcStreamer datastream;
		CcCallback1<CcPipeReaderProxy, CcPipeReader*> iOnOpen;
		CcCallback1<CcPipeReaderProxy, CcPipeReader*> iOnClose;
		CcCallback1<CcPipeReaderProxy, CcPipeReader*> iOnBroken;
		CcCallback1<CcPipeReaderProxy, CcPipeReader*> iOnRead;
	private:
	protected:
		CcPipeSink* _pipe;
		CcThreadSafe<> _isopen;
		std::string _filename;
		CcBuffer<>* _rbuff;
};

class CcPipeReaderProxy {
	public:
		virtual void HandleOpen(CcPipeReader* caller) { }
		virtual void HandleClose(CcPipeReader* caller) { }
		virtual void HandleBroken(CcPipeReader* caller) { }
		virtual void HandleRead(CcPipeReader* caller) { }
};

#define CB_CcPipeReader(sender_event, receiver_ptr, method) 		\
	sender_event.Register((CcPipeReaderProxy*)receiver_ptr, 		\
			&CcPipeReaderProxy::method);
#endif
