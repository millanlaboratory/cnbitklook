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

#ifndef XMLCONFIG_HPP
#define XMLCONFIG_HPP

#include "XMLException.hpp"
#include "XMLType.hpp"
#include "XMLDocument.hpp"
#include <assert.h>
#include <vector>
#include <utility>

/*! \brief Simplified interface to CCfgXMLConfig
 *
 * Provides a function to access the CNBI defined blocks.
 * This class is highly redundant and not really useful, but
 * it simplifies data access for my users.
 */
class CCfgXMLConfig {
	friend class CCfgConfig;

	public:
		CCfgXMLConfig(const std::string& filename,
				const std::string& rootname = "cnbiconfig");
		virtual ~CCfgXMLConfig(void);

		CCfgXMLConfig* Root(void);
		CCfgXMLConfig* Go(const std::string& blockname);
		CCfgXMLConfig* Quick(const std::string& blockpath);
		std::string GetRaw(void);
		CCfgXMLConfig* GetRaw(std::string* value);
		XMLType Get(void);
		CCfgXMLConfig* Get(XMLType* value);
		std::string GetAttr(const std::string& name);
		CCfgXMLConfig* SetBranch(void);
		CCfgXMLConfig* Branch(void);
		XMLNode Child(void);
		XMLNode NextSibling(void);
		void Dump(void);
		std::string QuickString(const std::string& str);
		bool QuickBool(const std::string& str);
		float QuickFloat(const std::string& str);
		int QuickInt(const std::string& str);
		unsigned int QuickUInt(const std::string& str);
	protected:
		void SearchRoot(void);
		void CheckRoot(void);
		void CheckBranch(void);
		void CheckLeaf(void);
	private:
		static void Tokenize(const std::string& str, 
				std::vector<std::string>& tokens, 
				const std::string& delimiters = "/");

	private:
		std::string _rootname;
		XMLDocument _document;
		XMLNode _nRoot;
		XMLNode _nBranch;
		XMLNode _nLeaf;
};

#endif
