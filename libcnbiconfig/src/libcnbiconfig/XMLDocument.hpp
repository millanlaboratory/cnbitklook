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
#ifndef XMLDOCUMENT_HPP
#define XMLDOCUMENT_HPP

#include <string>
#include <rapidxml.hpp>

typedef rapidxml::xml_node<>* XMLNode; 
typedef rapidxml::xml_attribute<>* XMLAttr; 

class XMLDocument {
	public:
		XMLDocument(unsigned int bsize = 8192);
		virtual ~XMLDocument();
		
		virtual void ImportFile(std::string filename);
		virtual void ImportBuffer(std::string buffer);

		virtual void Dump(void);
		virtual void Stats(void);

	protected:
		virtual void Parse(void);
		virtual void FillBuffer(std::string* buffer);

	public:
		rapidxml::xml_document<> doc;

	protected:
		unsigned int _bsize;
		char* _buffer;
};

#endif
