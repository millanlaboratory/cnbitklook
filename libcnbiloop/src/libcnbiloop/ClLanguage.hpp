/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLLANGUAGE_HPP 
#define CLLANGUAGE_HPP 

#include <libcnbicore/CcBuffer.hpp>

class ClLanguage {
	public:
		ClLanguage(size_t bsize = 4096, size_t csize = 2048);
		virtual ~ClLanguage(void);
		virtual size_t MaxSize(void);

	public:
		CcBuffer<char>* message;
	protected:
		size_t _bsize;
		CcBuffer<char>* _cache0;
		CcBuffer<char>* _cache1;
		CcBuffer<char>* _cache2;
		CcBuffer<char>* _cache3;
		CcBuffer<char>* _cache4;
		CcBuffer<char>* _cache5;
};

#endif
