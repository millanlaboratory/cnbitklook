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

#ifndef CCFILE_HPP 
#define CCFILE_HPP 

#include "CcBasic.hpp"
#include <string>

/*! \brief File wrapper
 *
 * This class simplifies file access by simplifying the read/write operations.
 */
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
