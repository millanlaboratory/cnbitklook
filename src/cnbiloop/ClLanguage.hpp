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

#ifndef CLLANGUAGE_HPP 
#define CLLANGUAGE_HPP 

#include <cnbicore/CcBuffer.hpp>

class ClLanguage {
	public:
		ClLanguage(size_t msize = 4092, size_t csize = 2048);
		virtual ~ClLanguage(void);
		size_t MessageSize(void);
		size_t CacheSize(void);

	public:
		CcBuffer<char>* message;
	protected:
		size_t _msize;
		CcBuffer<char>* _cache0;
		CcBuffer<char>* _cache1;
		CcBuffer<char>* _cache2;
		CcBuffer<char>* _cache3;
		CcBuffer<char>* _cache4;
		CcBuffer<char>* _cache5;
};

#endif
