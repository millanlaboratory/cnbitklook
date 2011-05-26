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

#ifndef CCPIPESOURCE_CPP 
#define CCPIPESOURCE_CPP 

#include "CcPipeSource.hpp" 
#include "CcBasic.hpp" 

CcPipeSource::CcPipeSource(const std::string& filename) : CcPipe(filename) {
}

CcPipeSource::~CcPipeSource(void) {
}

void CcPipeSource::Open(void) {
	this->_sempipe.Wait();
	CcLogConfig(std::string("Opening: ").append(this->_pipe.filename));
	if(tp_exist(&this->_pipe) != 0) {
		CcLogDebug(std::string("Creating: ").append(this->_pipe.filename));

		if(tp_create(&this->_pipe) < 0) {
			std::string message;
			message.append("Cannot create pipe: ");
			message.append(this->_pipe.filename);
			this->_sempipe.Post();
			CcThrow(message);
		}
	}
	
	if(tp_openwrite(&this->_pipe) <= 0) {
		std::string message;
		message.append("Cannot open pipe: ");
		message.append(this->_pipe.filename);
		this->_sempipe.Post();
		CcThrow(message);
	}
	
	this->_sempipe.Post();
}

void CcPipeSource::Open(size_t size) {
	this->Open();
	if(size) {
		this->_sempipe.Wait();
		int a = tp_setsize(&this->_pipe, size);
		CcLogFatalS(">>>>>>>>>>>>> " << a);
		this->_sempipe.Post();
	}
}

void CcPipeSource::Close(void) {
	CcLogConfig(std::string("Closing: ").append(this->_pipe.filename));
	this->_sempipe.Wait();
	if(tp_exist(&this->_pipe) != 0) {
		std::string message;
		message.append("Pipe does not exist, cannot close it: ");
		message.append(this->_pipe.filename);
		this->_sempipe.Post();
		CcThrow(message);
	}

	if(tp_close(&this->_pipe) > -1) {
		this->_sempipe.Post();
		return;
	}
		
	std::string message;
	message.append("Cannot close pipe: ");
	message.append(this->_pipe.filename);
	this->_sempipe.Post();
	CcThrow(message);
}

size_t CcPipeSource::Write(const void* buffer, const size_t bsize) {
	size_t wsize;

	this->_sempipe.Wait();
	wsize = tp_write(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return wsize;
}
		
bool CcPipeSource::Write(const void* buffer, const size_t bsize, 
		size_t* wsize) {
	if(this->_sempipe.TryWait() == false) {
		*wsize = 0;
		return false;
	}
	*wsize = tp_write(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return true;
}
		
bool CcPipeSource::IsBroken(void) {
	return(tp_receivedsigpipe() == 1);
}
		
void CcPipeSource::CatchSIGPIPE(void) {
	tp_catchsigpipe();
}

void CcPipeSource::Clean(void) {
	this->_sempipe.Wait();
	tp_close(&this->_pipe);
	tp_remove(&this->_pipe);
	this->_sempipe.Post();
}
		

#endif
