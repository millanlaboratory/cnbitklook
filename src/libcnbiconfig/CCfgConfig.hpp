/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of LibCCfgConfig
 */

#ifndef CCFGCONFIG_HPP
#define CCFGCONFIG_HPP

#include <string>
#include "CCfgXMLConfig.hpp"
#include "CCfgTaskset.hpp"
#include <libtobiic/ICMessage.hpp>
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
