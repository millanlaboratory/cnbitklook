/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libxmlconfig library

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

#ifndef XMLTYPE_HPP
#define XMLTYPE_HPP

#include <string>
#include "XMLException.hpp"

typedef int XMLTypeData;

class XMLType {
	public:
		XMLType(bool verbose = false);
		XMLType(std::string buffer, bool verbose = false);
		XMLType(const char* buffer, bool verbose = false);
		virtual ~XMLType(void);
	
		virtual void SetKey(std::string key);
		virtual void SetKey(const char* key);
		virtual std::string Key(void);
		virtual void SetId(std::string key);
		virtual void SetId(const char* id);
		virtual std::string Id(void);

		virtual XMLTypeData Guess(std::string buffer);
		virtual XMLTypeData Guess(const char* buffer);

		virtual bool Bool(void);
		virtual float Float(void);
		virtual int  Int(void);
		virtual int HexInt(void);
		virtual std::string String(void);

	public:
		static const XMLTypeData TypeNone   = 0;
		static const XMLTypeData TypeBool   = 1;
		static const XMLTypeData TypeFloat  = 2;
		static const XMLTypeData TypeInt    = 3;
		static const XMLTypeData TypeString = 4;
		static const XMLTypeData TypeHexInt = 5;

	protected:
		std::string _key;
		std::string _id;
		
		bool _vbool;
		float _vfloat;
		int _vint;
		std::string _vstring;
		unsigned int _vuint;

		int _type;
		bool _verbose;
};

#endif
