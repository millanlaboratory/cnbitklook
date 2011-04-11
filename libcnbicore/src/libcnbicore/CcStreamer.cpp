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

#ifndef CCSTREAMER_CPP
#define CCSTREAMER_CPP

#include "CcStreamer.hpp"

CcStreamer::CcStreamer(void) {
	CcObject::SetName("CcStreamer");
}

CcStreamer::~CcStreamer(void) {
}

void CcStreamer::Append(std::string buffer) {
	this->_mtxstream.Wait();
	this->_stream.append(buffer);
	this->_mtxstream.Post();
}
		
void CcStreamer::Append(const char* buffer, size_t bsize) {
	this->_mtxstream.Wait();
	this->_stream.append(buffer, bsize);
	this->_mtxstream.Post();
}

bool CcStreamer::Extract(std::string *buffer, std::string hdr, std::string trl, 
		CcStreamerDirection direction) {
	this->_mtxstream.Wait();
	
	
	if(this->_stream.empty()) {
		this->_mtxstream.Post();
		return false;
	}
	
	if(this->ImplHas(hdr, trl, direction) == false) {
		this->_mtxstream.Post();
		return false;
	}
	
	buffer->clear();

	std::string::size_type p_hdr, p_trl, delta;

	if(direction == CcStreamer::Forward) {
		p_hdr = this->_stream.find(hdr);
		p_trl = this->_stream.find(trl);
	} else {
		p_hdr = this->_stream.rfind(hdr);
		p_trl = this->_stream.rfind(trl);
	}
	delta = trl.size();

	if(p_hdr == std::string::npos || p_trl == std::string::npos) {
		this->_mtxstream.Post();
		return false;
	}
	
	if(p_hdr >= p_trl) {
		this->_mtxstream.Post();
		return false;
	}

	/* 2010-03-26  Matteo Lostuzzo <matteo.lostuzzo@epfl.ch>
	 * 2010-03-26  Michele Tavella <michele.tavella@epfl.ch>
	 * Extreme bug was blasted with Lostuzzo.
	 */
	*buffer = this->_stream.substr(p_hdr, p_trl - p_hdr + delta);
	this->_stream.erase(p_hdr, p_trl - p_hdr + delta);
	this->_mtxstream.Post();

	return true;
}
		
bool CcStreamer::Has(std::string hdr, std::string trl, 
		CcStreamerDirection direction) {
	bool result = false;
	this->_mtxstream.Wait();
	result = this->ImplHas(hdr, trl, direction);
	this->_mtxstream.Post();
	return result;
}
		
int CcStreamer::Count(std::string hdr) {
	int count = 0;
	
	this->_mtxstream.Wait();
	if(!this->_stream.empty()) {
		std::string::size_type pos(0);

		while (pos != std::string::npos) {
			pos = this->_stream.find(hdr, pos);
			if(pos != std::string::npos) {
				++count;
				pos += 3;
			}
		}
	}
	this->_mtxstream.Post();
	
	return count;
}

void CcStreamer::Dump(void) {
	this->_mtxstream.Wait();
	std::cout << "[CcStreamer::Dump] " << this->_stream  << std::endl;
	this->_mtxstream.Post();
}

int CcStreamer::Size(void) {
	int size = 0;
	this->_mtxstream.Wait();
	size = this->_stream.size();
	this->_mtxstream.Post();

	return size;
}

void CcStreamer::Clear(void) {
	this->_mtxstream.Wait();
	this->_stream.clear();
	this->_mtxstream.Post();
}

bool CcStreamer::ImplHas(std::string hdr, std::string trl, 
		CcStreamerDirection direction) {
	
	if(this->_stream.empty())
		return false;

	std::string::size_type p_hdr, p_trl;

	if(direction == CcStreamer::Forward) {
		p_hdr = this->_stream.find(hdr);
		p_trl = this->_stream.find(trl);
	} else {
		p_hdr = this->_stream.rfind(hdr);
		p_trl = this->_stream.rfind(trl);
	}

	if(p_hdr == std::string::npos || p_trl == std::string::npos) 
		return false;
	
	if(p_hdr >= p_trl) 
		return false;

	return true;
}

#endif
