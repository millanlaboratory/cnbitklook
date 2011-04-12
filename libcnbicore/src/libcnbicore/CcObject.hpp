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

#ifndef CCOBJECT_HPP
#define CCOBJECT_HPP

#include "CcCore.hpp"
#include <string>
#include <iostream>

/*! \brief The LibCNBICore base object
 *
 * All the classes in LibCNBICore inherit from CcObject. 
 * Nothing special for the time being.
 * If you are handling streams, I strongly suggest you use this class.
 */
class CcObject {
	public:
		CcObject(const char* name = "unset", bool verbose = false);
		virtual ~CcObject(void);
		CcObject* SetName(std::string name);
		std::string GetName(void) const;
		unsigned int GetInstances(void) const;
		void SetVerbose(bool verbose);
		bool IsVerbose(void) const;
		const void DumpInfo(void) const;

	private:
		std::string _objectName;
		std::string _objectTimestamp;
		std::string _objectDatetime;
		static unsigned int _objectInstances;
		unsigned int _objectId;
		bool _verbose;
		CcCore* _core;

	public:
		friend std::ostream &operator<<(std::ostream& output, const CcObject&);
		friend std::ostream &operator<<(std::ostream& output, const CcObject*);
		bool operator==(const CcObject& right);
		bool operator!=(const CcObject& right);

		friend class CcCore;
};

#endif
