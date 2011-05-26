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

#ifndef CCPIPESERVER_CPP 
#define CCPIPESERVER_CPP 

#include "CcPipeServer.hpp" 
#include "CcBasic.hpp"
#include <string>
#include <stdio.h>

#include <iostream>

CcPipeServer::CcPipeServer(void* ackbuffer, size_t ackbsize, size_t bsize) {
	CcPipeSource::CatchSIGPIPE();
	this->_isopen.Set(false);

	if(ackbuffer != NULL && ackbsize != 0) {
		this->_ackbsize = ackbsize;
		this->_ackbuffer = malloc(this->_ackbsize);
		memcpy(this->_ackbuffer, ackbuffer, this->_ackbsize);
	} else {
		this->_ackbuffer = NULL;
		this->_ackbsize = 0;
	}
	this->_bsize = bsize;
}

CcPipeServer::~CcPipeServer(void) {
	if(this->_ackbuffer != NULL)
		free(this->_ackbuffer);
}

void CcPipeServer::Open(const std::string& filename, unsigned int total) {
	if(this->_isopen.Get() == true)
		return;

	char buf[32];
	std::string pipename;
	for(unsigned int i = 0; i < total; i++) {
		sprintf(buf, "%d", i);
		pipename.assign(filename);
		pipename.append(buf);
		CcPipeWriter* w = new CcPipeWriter(this->_bsize);
		this->_pipes.push_back(w);
		this->_pipes.at(i)->Acknoledge(this->_ackbuffer, this->_ackbsize);
		this->_pipes.at(i)->Open(pipename);
	}
	this->_isopen.Set(true);
}

void CcPipeServer::Close(void) {
	if(this->_isopen.Get() == false)
		return;
	this->_isopen.Set(false);
	
	CcPipeWriter* w;
	std::vector<CcPipeWriter*>::iterator it;
	
	for(it = this->_pipes.begin(); it != this->_pipes.end(); it++) {
		w = (*it);
		
		if(w->IsBroken() == true) {
			w->Close();
			delete w;
			w = NULL;
			continue;
		} else if(w->IsOpen() == true) {
			w->Close();	
			delete w;
			w = NULL;
			continue;
		} else { 
			std::string message;
			message.append("Stuck: ");
			message.append(w->GetFilename());
			CcLogWarning(message);
		}
	}
}
		
void CcPipeServer::Write(const void* buffer, size_t bsize) {
	CcPipeWriter* w;
	std::vector<CcPipeWriter*>::iterator it;

	if(this->_isopen.Get() == false) 
		CcThrow("Cannot write if closing or not opened");

	for(it = this->_pipes.begin(); it != this->_pipes.end(); it++) {
		w = (*it);
		if(w->IsBroken() == true) {
			w->Open();
			continue;
		}
		if(w->IsOpen() == false)
			continue;
		if(this->_bsize == 0) 
			w->Write(buffer, bsize);
		else
			w->BWrite(buffer);
	}
}

#endif
