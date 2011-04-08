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

#ifndef CCFGTASK_HPP
#define CCFGTASK_HPP

#include <string>
#include "CCfgTypes.hpp"
#include "libxmlconfig/SmartType.hpp"
#include <map>

typedef std::map<std::string, SmartType> CCfgTaskConfig;
typedef std::map<std::string, SmartType>::iterator CCfgTaskConfigIt;

/*! \brief Definition of a Task according to the CNBI XML definitions
 *
 * It is more or less a structure, nothing more than this.
 * It contains the informations that characterize a task, such as the GDF
 * events, the HW triggers, the bias, the threshold and so on. 
 * The definition of task comes from the MI tasks.
 */
class CCfgTask {
	public:
		//! \brief Constructor
		CCfgTask(void);
		//! \brief Dumps the public methods to stdo
		void Dump(void);

	public:
		//! \brief Task name (i.e. mi_hand_right)
		std::string    	name;
		//! \brief Task description (i.e. Right_Hand_MI)
		std::string    	description;
		//! \brief Task ID (i.e. 0, 1, 2, ... k-1)
		unsigned int   	id;
		//! \brief Number of trials for the current task
		unsigned int   	trials;
		//! \brief GDFEvent associated with the task (i.e. 0x301)
		GDFEvent 		gdf;
		//! \brief HWTrigger associated with the task (i.e. 0 - 2^8-1)
		HWTrigger  		hwt;

		//! Holds all the other parameters (threshold, bias, etc.)
		CCfgTaskConfig config;
};

#endif
