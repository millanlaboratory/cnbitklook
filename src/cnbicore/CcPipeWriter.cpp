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
	CcPipeSource::CatchSIGPIPE();
	this->_isopen.Set(false);
	this->_pipe = NULL;
	this->_ackbsize = 0;
	this->_ackbuffer = NULL;
	if(bsize) {
		this->_wbuff = new CcDoubleBuffer(bsize);
		this->_bufferedmode.Set(true);
	} else {
		this->_wbuff = NULL;
		this->_bufferedmode.Set(false);
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
	this->_pipe = new CcPipeSource(filename);
	CcThread::Start();
	this->iOnOpen.Execute((CcPipeWriter*)this);
}
		
void CcPipeWriter::Open(void) { 
	if(this->_filename.empty() == true)
		CcThrow("Filename not set yet");
	this->Open(this->_filename);
}

void CcPipeWriter::Close(void) {
	if(CcThread::IsRunning() == false)
		return;

	CcThread::Stop();
	CcThread::Join();

	if(this->_pipe != NULL)
		delete this->_pipe;
	this->_pipe = NULL;
	this->iOnClose.Execute((CcPipeWriter*)this);
}
		
void CcPipeWriter::Main(void) {
	CcLogDebug("Starting thread");
	
	this->_isopen.Set(false);
	try { 
		this->_pipe->Open(); 
	} catch(CcException e) { 
		this->Stop();
		return;
	}
	this->_isopen.Set(true);

	std::string message;
	message.append("Attached: ");
	message.append(this->_pipe->GetFilename());
	CcLogConfig(message);

	if(this->_ackbuffer != NULL) {
		CcLogInfo("Sending ACK");
		this->Write(this->_ackbuffer, this->_ackbsize);
	}

	if(this->_bufferedmode.Get() == true) {
		CcBuffer<>* cache = new CcBuffer<>(this->_wbuff->Bsize());
		while(CcThread::IsRunning()) {
			this->_wbuff->Read(cache->buffer);
			this->Write(cache->buffer, cache->bsize);
		}
	} 
	
	if(this->_bufferedmode.Get() == false) {
		while(CcThread::IsRunning()) {
			CcTime::Sleep(5.00f);
		}
	}
	CcLogDebug("Stopping thread");
	try { this->_pipe->Close(); } catch(CcException e) { }
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
		std::string message;
		message.append("Broken: ");
		message.append(this->_pipe->GetFilename());
		CcLogWarning(message);
		CcThread::Stop();
		this->iOnBroken.Execute((CcPipeWriter*)this);
	} else 
		this->iOnWrite.Execute((CcPipeWriter*)this);
	return result;
}
		
size_t CcPipeWriter::Write(const char* buffer) {
	return this->Write(buffer, (const size_t)strlen(buffer));
}

size_t CcPipeWriter::Write(std::string& buffer) {
	return this->Write(buffer.c_str(), (const size_t)buffer.size());
}
		
void CcPipeWriter::BWrite(const void* buffer) {
	if(this->_bufferedmode.Get() == false)
		CcThrow("CcPipeWriter is not configured for buffered writing");

	this->_wbuff->Write(buffer);
}

std::string CcPipeWriter::GetFilename(void) {
	return this->_pipe->GetFilename();
}
		
void CcPipeWriter::Acknoledge(const void* buffer, const size_t bsize) {
	if(this->_ackbuffer != NULL)
		free(this->_ackbuffer);
	
	this->_ackbsize = bsize;
	this->_ackbuffer = malloc(this->_ackbsize);
	memcpy(this->_ackbuffer, buffer, bsize);
}

#endif
