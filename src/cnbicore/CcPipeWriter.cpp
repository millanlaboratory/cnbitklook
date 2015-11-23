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

#ifndef CCPIPEWRITER_CPP 
#define CCPIPEWRITER_CPP 

#include "CcPipeWriter.hpp" 

CcPipeWriter::CcPipeWriter(size_t bsize) {
	CcPipe::CatchSIGPIPE();
	this->_isopen.Set(false);
	this->_pipe = NULL;
	this->_ackbsize = 0;
	this->_ackbuffer = NULL;
	if(bsize > 0) {
		this->_wbuff = new CcDoubleBuffer(bsize);
	} else {
		this->_wbuff = NULL;
	}
}

CcPipeWriter::~CcPipeWriter(void) {
	if(this->_pipe != NULL)
		delete this->_pipe;
	if(this->_ackbuffer != NULL)
		free(this->_ackbuffer);
	if(this->_wbuff != NULL)
		delete this->_wbuff;
}

void CcPipeWriter::Open(const std::string& filename) {
	if(this->_isopen.Get())
		return;

	this->_filename.assign(filename);
	this->_pipe = new CcPipe();
	CcThread::Start();
	this->iOnOpen.Execute(this);
}
		
void CcPipeWriter::Close(void) {
	if(CcThread::IsRunning() == false)
		return;

	CcThread::Stop();
	CcThread::Join();

	if(this->_pipe != NULL)
		delete this->_pipe;
	this->_pipe = NULL;
	this->iOnClose.Execute(this);
}
		
void CcPipeWriter::Main(void) {
	if(this->_pipe->Open(this->_filename, CcPipe::Writer) == false) {
		this->_isopen.Set(false);
		this->Stop();
		return;
	}
	this->_isopen.Set(true);
	CcLogDebugS("Writer attached: " << this->_filename);

	if(this->_ackbuffer != NULL) {
		CcLogInfoS("Writing acknoledgment packet: " << this->_filename << 
				", " << this->_ackbsize << " bytes");
		this->Write(this->_ackbuffer, this->_ackbsize);
	}

	if(this->_wbuff != NULL) {
		CcBuffer<>* cache = new CcBuffer<>(this->_wbuff->Bsize());
		while(CcThread::IsRunning()) {
			this->_wbuff->Read(cache->buffer);
			this->Write(cache->buffer, cache->bsize);
		}
	} 
	
	if(this->_wbuff == NULL)
		while(CcThread::IsRunning())
			CcTime::Sleep(5.00f);

	this->_pipe->Close();
	this->_isopen.Set(false);
}

bool CcPipeWriter::IsOpen(void) {
	return this->_isopen.Get();
}

bool CcPipeWriter::IsBroken(void) {
	return !CcThread::IsRunning();
}

size_t CcPipeWriter::Write(const void* buffer, const size_t bsize) {
	size_t result = this->_pipe->Write(buffer, bsize);
	if(result == TP_BROKENS) {
		CcLogWarningS("Pipe is broken: " << this->_filename);
		CcThread::Stop();
		this->iOnBroken.Execute(this);
	} else 
		this->iOnWrite.Execute(this);
	return result;
}
		
size_t CcPipeWriter::Write(const char* buffer) {
	return this->Write(buffer, (const size_t)strlen(buffer));
}

size_t CcPipeWriter::Write(std::string& buffer) {
	return this->Write(buffer.c_str(), (const size_t)buffer.size());
}
		
bool CcPipeWriter::BufferedWrite(const void* buffer) {
	if(this->_wbuff == NULL) {
		CcLogError("Writer not configured for buffered writing");
		return false;
	}
	this->_wbuff->Write(buffer);
	return true;
}

std::string CcPipeWriter::GetFilename(void) {
	return this->_filename;
}
		
void CcPipeWriter::Acknoledge(const void* buffer, const size_t bsize) {
	if(this->_ackbuffer != NULL)
		free(this->_ackbuffer);
	
	this->_ackbsize = bsize;
	this->_ackbuffer = malloc(this->_ackbsize);
	memcpy(this->_ackbuffer, buffer, bsize);
}

#endif
