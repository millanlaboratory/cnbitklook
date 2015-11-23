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

#ifndef CCPIPE_CPP 
#define CCPIPE_CPP 

#include "CcPipe.hpp" 

CcPipe::CcPipe(void) {
	this->_mode = CcPipe::Unset;
}

CcPipe::~CcPipe(void) {
	this->Close();
}

bool CcPipe::Open(const std::string& filename, int mode) {
	this->_sempipe.Wait();
	tp_init(&this->_pipe, filename.c_str());
	bool status;
	switch(mode) {
		case CcPipe::Reader:
			status = this->OpenRead();
			break;
		case CcPipe::Writer:
			status = this->OpenWrite();
			break;
		default:
		case CcPipe::Unset:
			status = false;
			break;
	}
	this->_sempipe.Post();
	return(status);
}

bool CcPipe::OpenRead(void) {
	int status = tp_openread(&this->_pipe);
	this->_mode = CcPipe::Reader;
	return status;
}

bool CcPipe::OpenWrite(void) {
	if(tp_exist(&this->_pipe) != 0) {
		CcLogDebugS("Creating writer: " << this->_pipe.filename);

		if(tp_create(&this->_pipe) < 0) {
			CcLogErrorS("Cannot create writer: " << this->_pipe.filename);
			return false;
		}
	}
	
	CcLogDebugS("Waiting for reader to attach: " << this->_pipe.filename);
	if(tp_openwrite(&this->_pipe) <= 0) {
		CcLogErrorS("Cannot open writer: " << this->_pipe.filename);
		return false;
	}
	
	CcLogInfoS("Reader attached to writer: " << this->_pipe.filename);
	this->_mode = CcPipe::Writer;
	return true;
}

bool CcPipe::Close(void) {
	this->_sempipe.Wait();
	if(tp_exist(&this->_pipe) != 0) {
		CcLogWarningS("Pipe does not exist: " << this->_pipe.filename);
		this->_sempipe.Post();
	}

	if(tp_close(&this->_pipe) > -1) {
		CcLogDebugS("Pipe closed: " << this->_pipe.filename);
		if(this->_mode == CcPipe::Writer) {
			if(tp_remove(&this->_pipe) < 0)
				CcLogErrorS("Cannot remove writer: " << this->_pipe.filename);
			CcLogDebugS("Removed writer: " << this->_pipe.filename);
		}
		this->_sempipe.Post();
		return true;
	}

	CcLogErrorS("Cannot close pipe: " << this->_pipe.filename);
	this->_sempipe.Post();
	return false;
}
		
ssize_t CcPipe::Write(const void* buffer, const size_t bsize) {
	size_t size;
	this->_sempipe.Wait();
	size = tp_write(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return size;
}

ssize_t CcPipe::TryWrite(const void* buffer, const size_t bsize) {
	if(this->_sempipe.TryWait() == false)
		return -1;
	
	size_t size = tp_write(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return size;
}

ssize_t CcPipe::Read(void* buffer, const size_t bsize) {
	size_t size;
	this->_sempipe.Wait();
	size = tp_read(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return size;
}

ssize_t CcPipe::TryRead(void* buffer, const size_t bsize) {
	if(this->_sempipe.TryWait() == false)
		return -1;
	
	size_t size = tp_read(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return size;
}

void CcPipe::CatchSIGPIPE(void) {
	tp_catchsigpipe();
}

bool CcPipe::IsBroken(void) {
	return(tp_receivedsigpipe() == 1);
}
		
#endif
