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
typedef CCfgTasksetMap::iterator CCfgTasksetIt;
typedef CCfgTasksetMap::const_iterator	CCfgTasksetConstIt;

/*! \brief The taskset is a map of CCfgTask objects sorted by the GDFEvent
 * 
 * A set of CCfgTask constitutes a CCfgTaskset, which simply wraps a
 * CCfgTasksetMap object.
 */
class CCfgTaskset {
	public: 
		CCfgTaskset(const std::string& name);
		virtual CCfgTask* AddTask(CCfgTask* task);
		virtual CCfgTask* RemoveTaskEx(const std::string& name);
		virtual CCfgTask* GetTaskEx(const std::string& name);
		virtual CCfgTask* GetTaskEx(unsigned int id);
		virtual bool HasTask(std::string name);
		virtual unsigned int Count(void);
		virtual void Clear(void);

		CCfgTasksetIt Begin(void);
		CCfgTasksetIt End(void);
		CCfgTasksetConstIt Begin(void) const;
		CCfgTasksetConstIt End(void) const;

	public:
		CCfgTasksetMap tasks;
		std::string name;
		std::string description;
		std::string classifier;
};

#endif
