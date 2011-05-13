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

#ifndef CCPIPEREADER_CPP 
#define CCPIPEREADER_CPP 

#include "CcPipeReader.hpp" 

CcPipeReader::CcPipeReader(size_t bsize) {
	this->_isopen.Set(false);
	this->_pipe = NULL;
	this->_rbuff = new CcBuffer<>(bsize);
}

CcPipeReader::~CcPipeReader(void) {
	if(this->_pipe != NULL)
		delete this->_pipe;
	if(this->_rbuff != NULL)
		delete this->_rbuff;
}

void CcPipeReader::Open(const std::string& filename) {
	if(this->_isopen.Get())
		return;
	this->_filename.assign(filename);
	this->_pipe = new CcPipeSink(filename);
	this->Start();
	this->iOnOpen.Execute((CcPipeReader*)this);
}

void CcPipeReader::Close(void) {
	if(CcThread::IsRunning() == false)
		return;

	this->Stop();
	this->Join();
	this->iOnClose.Execute((CcPipeReader*)this);
	
	if(this->_pipe == NULL)
		return;
	delete this->_pipe;
	this->_pipe = NULL;
}

void CcPipeReader::Main(void) {
	CcLogDebug("Starting thread");
	
	this->_isopen.Set(false);
	try { this->_pipe->Open(); } catch(CcException e) { this->Stop(); }
	this->_isopen.Set(true);
	
	std::string message;
	message.append("Attached: ");
	message.append(this->_pipe->GetFilename());
	CcLogConfig(message);
	
	size_t rsize;
	while(CcThread::IsRunning()) {
		rsize = this->_pipe->Read(this->_rbuff->buffer, this->_rbuff->bsize);

		if(rsize <= 0) {
			CcLogWarning("Broken");
			CcThread::Stop();
			this->iOnBroken.Execute((CcPipeReader*)this);
			break;
		}
		this->datastream.Append((const char*)this->_rbuff->buffer, rsize);
		this->iOnRead.Execute((CcPipeReader*)this);
	}
	
	CcLogDebug("Stopping thread");
	try { this->_pipe->Close(); } catch(CcException e) { }
	this->_isopen.Set(false);
}

bool CcPipeReader::IsBroken(void) {
	return this->_isopen.Get();
}

bool CcPipeReader::IsOpen(void) {
	return !CcThread::IsRunning();
}


#endif
