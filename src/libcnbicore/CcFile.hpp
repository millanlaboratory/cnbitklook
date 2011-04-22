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

#ifndef CCFILE_HPP 
#define CCFILE_HPP 

#include "CcBasic.hpp"
#include <string>

class CcFile : public CcObject {
	public:
		CcFile(void);
		virtual ~CcFile(void);
		virtual bool Load(const std::string& filename);
		virtual bool Save(const std::string& filename);
		virtual void Set(const std::string& data);
		virtual void Get(std::string* data);
		virtual std::string Get(void);
		virtual void Clear(void);
		virtual size_t Size(void);
		virtual size_t MaxSize(void);

	protected:
		std::string _data;
		CcSemaphore _semdata;
};

#endif
