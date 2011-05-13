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

#ifndef CCPIPESINK_CPP 
#define CCPIPESINK_CPP 

#include "CcPipeSink.hpp" 
#include "CcBasic.hpp"

CcPipeSink::CcPipeSink(const std::string& filename) : CcPipe(filename) {
}

CcPipeSink::~CcPipeSink(void) {
}

void CcPipeSink::Open(void) {
	this->Open(-1);
}

void CcPipeSink::Open(double wait) {
	this->_sempipe.Wait();
	int status;
	if(wait < 0)	
		CcLogDebug("Waiting for pipe");
			
	CcLogConfig(std::string("Attaching: ").append(this->_pipe.filename));

	CcTimeValue tic;
	CcTime::Tic(&tic);
	while(true) {
		status = tp_openread(&this->_pipe);
			std::string message;
		
		if(status > 0)
			break;
		
		if(status <= 0 && wait == 0) {
			std::string message;
			message.append("Cannot open pipe: ");
			message.append(this->_pipe.filename);
			this->_sempipe.Post();
			CcThrow(message);
		}

		if(CcTime::Toc(&tic) >= wait && status <= 0 && wait > 0) {
			std::string message;
			message.append("Cannot open pipe after waiting: ");
			message.append(this->_pipe.filename);
			this->_sempipe.Post();
			CcThrow(message);
		}
		CcTime::Sleep(250.00f);
	}
	this->_sempipe.Post();
}

void CcPipeSink::Close(void) {
	this->_sempipe.Wait();
	CcLogConfig(std::string("Detaching: ").append(this->_pipe.filename));
	if(tp_close(&this->_pipe) >= 0) {
		this->_sempipe.Post();
		return;
	}
	
	std::string message;
	message.append("Cannot close pipe: ");
	message.append(this->_pipe.filename);
	this->_sempipe.Post();
	CcThrow(message);
}
		
size_t CcPipeSink::Read(void* buffer, const size_t bsize) {
	size_t rsize;
	this->_sempipe.Wait();
	rsize = tp_read(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return rsize;
}

bool CcPipeSink::Read(void* buffer, const size_t bsize, size_t* rsize) {
	if(this->_sempipe.TryWait() == false) {
		*rsize = 0;
		return false;
	}
	*rsize = tp_read(&this->_pipe, buffer, bsize);
	this->_sempipe.Post();
	return true;
}

#endif
