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

#ifndef SMARTTYPE_HPP
#define SMARTTYPE_HPP

#include <string>
#include "XMLException.hpp"

typedef int SmartData;

class SmartType {
	public:
		SmartType(bool verbose = false);
		SmartType(std::string buffer, bool verbose = false);
		SmartType(const char* buffer, bool verbose = false);
		virtual ~SmartType(void);
	
		virtual void SetKey(std::string key);
		virtual void SetKey(const char* key);
		virtual std::string Key(void);
		virtual void SetId(std::string key);
		virtual void SetId(const char* id);
		virtual std::string Id(void);

		virtual SmartData Guess(std::string buffer);
		virtual SmartData Guess(const char* buffer);

		virtual bool Bool(void);
		virtual float Float(void);
		virtual int  Int(void);
		virtual unsigned int UInt(void);
		virtual std::string String(void);

	public:
		static const SmartData TypeNone   = 0;
		static const SmartData TypeBool   = 1;
		static const SmartData TypeFloat  = 2;
		static const SmartData TypeInt    = 3;
		static const SmartData TypeString = 4;
		static const SmartData TypeUInt   = 5;

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
