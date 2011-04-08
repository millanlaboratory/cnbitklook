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
		CCfgConfig(const std::string& filename);
		virtual ~CCfgConfig(void);
		virtual bool Validate(void);

		virtual CCfgTaskset* Online(const std::string& blockname, 
				const std::string& taskset, ICMessage* icmessage = NULL);
		virtual CCfgTaskset* Offline(const std::string& blockname,
				const std::string& taskset);

	private:
		virtual void ParseTaskset(const std::string& name,
				CCfgTaskset* taskset);
		virtual void ParseConfig(const std::string& mode,
				const std::string& modename, const std::string& tasksetname,
				CCfgTaskset* taskset);
		virtual void ParseClassifier(const std::string& modename, 
				const std::string& tasksetname, CCfgTaskset* taskset,
				ICMessage* icmessage);
};

#endif
