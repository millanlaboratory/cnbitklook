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

CcPipe::CcPipe(const std::string& filename) {
	tp_init(&this->_pipe, filename.c_str());
}

CcPipe::~CcPipe(void) {
	Clean();
}
		
void CcPipe::Clean(void) {
	this->_sempipe.Wait();
	tp_close(&this->_pipe);
	this->_sempipe.Post();
}
		
std::string CcPipe::GetFilename(void) {
	this->_sempipe.Wait();
	std::string filename(this->_pipe.filename);
	this->_sempipe.Post();
	return filename;
}


#endif
