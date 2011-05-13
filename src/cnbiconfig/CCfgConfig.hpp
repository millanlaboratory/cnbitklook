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

#ifndef CCFGCONFIG_HPP
#define CCFGCONFIG_HPP

#include <string>
#include "CCfgXMLConfig.hpp"
#include "CCfgTaskset.hpp"
#include <tobiic/ICMessage.hpp>
//#include "CCfgTypes.hpp"

/*! \brief Parser for nested blocks according to the CNBI XML definitions
 *
 * Provides the methods needed to build a CCfgTaskset object starting from the
 * CNBI XML configuration file.
 * Moreover, it implements all the complex method to parse nested blocks.
 */
class CCfgConfig : public CCfgXMLConfig {
	public:
		bool Validate(void);
		CCfgTaskset* OnlineEx(const std::string& blockname, 
				const std::string& taskset, ICMessage* icmessage = NULL);
		CCfgTaskset* OfflineEx(const std::string& blockname,
				const std::string& taskset);

	private:
		void ParseTasksetEx(const std::string& name,
				CCfgTaskset* taskset);
		void ParseConfigEx(const std::string& mode,
				const std::string& bname, const std::string& tname,
				CCfgTaskset* taskset);
		void ParseClassifierEx(const std::string& bl, const std::string& ts,
				CCfgTaskset* taskset, ICMessage* icmessage);
};

#endif
