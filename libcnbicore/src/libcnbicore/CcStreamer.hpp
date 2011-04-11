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
#ifndef CCSTREAMER_HPP
#define CCSTREAMER_HPP

#include "CcObject.hpp"
#include "CcSemaphore.hpp"
#include <string>

typedef unsigned int CcStreamerDirection;

/*! \brief Data streams handler
 *
 * This class is designed to allow an easy handling of data streams.
 * It allows to verify whether a certain message is present and to extract it
 * automatically. 
 */
class CcStreamer : public CcObject {
	public:
		CcStreamer(void);
		virtual ~CcStreamer(void);

		virtual void Append(std::string buffer);
		virtual void Append(const char* buffer, size_t bsize);
		virtual bool Extract(std::string* buffer, std::string hdr, 
				std::string trl, CcStreamerDirection direction);
		virtual bool Has(std::string hdr, std::string trl, 
				CcStreamerDirection direction);
		virtual int Count(std::string hdr);
		virtual void Dump(void);
		virtual int Size(void);
		virtual void Clear(void);
	private:
		virtual bool ImplHas(std::string hdr, std::string trl, 
				CcStreamerDirection direction);

	private:
		std::string _stream;
		CcSemaphore _mtxstream;

	public:
		static const CcStreamerDirection Forward = 0;
		static const CcStreamerDirection Reverse = 1;
};

#endif
