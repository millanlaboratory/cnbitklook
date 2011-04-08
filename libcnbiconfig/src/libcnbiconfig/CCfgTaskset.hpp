/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiconfig library

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
#ifndef CCFGTASKSET_HPP
#define CCFGTASKSET_HPP

#include <map>
#include "CCfgTypes.hpp"
#include "CCfgTask.hpp"

typedef std::map<std::string, CCfgTask*> CCfgTasksetMap;
typedef CCfgTasksetMap::iterator CCfgTasksetIter;
typedef CCfgTasksetMap::const_iterator	CCfgTasksetConstIter;

/*! \brief The taskset is a map of CCfgTask objects sorted by the GDFEvent
 * 
 * A set of CCfgTask constitutes a CCfgTaskset, which simply wraps a
 * CCfgTasksetMap object.
 */
class CCfgTaskset {
	public: 
		virtual CCfgTask* Add(CCfgTask* task);
		virtual CCfgTask* Remove(std::string name);
		virtual CCfgTask* Get(std::string name);
		virtual CCfgTask* Get(unsigned int id);
		virtual bool Has(std::string name);
		virtual unsigned int Count(void);
		virtual void Clear(void);

		CCfgTasksetIter Begin(void);
		CCfgTasksetIter End(void);
		CCfgTasksetConstIter Begin(void) const;
		CCfgTasksetConstIter End(void) const;

	public:
		CCfgTasksetMap tasks;
		std::string description;
		std::string classifier;
};

#endif
