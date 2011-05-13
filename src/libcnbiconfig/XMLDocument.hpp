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

#ifndef XMLDOCUMENT_HPP
#define XMLDOCUMENT_HPP

#include <string>
#include <rapidxml.hpp>

typedef rapidxml::xml_node<>* XMLNode; 
typedef rapidxml::xml_attribute<>* XMLAttr; 

class XMLDocument {
	public:
		XMLDocument(size_t bsize = 20480);
		virtual ~XMLDocument();
		int ImportFile(const std::string& filename);
		int ImportBuffer(const std::string& buffer);
		void Dump(void);
		void Stats(void);
	protected:
		virtual int Parse(void);
		virtual int FillBuffer(const std::string* buffer);

	public:
		rapidxml::xml_document<> doc;
	public:
		const static int Successfull = 0;
		const static int FileInputError = 1;
		const static int BufferOverflow = 2;
		const static int ParsingError = 3;

	protected:
		unsigned int _bsize;
		char* _buffer;
};

#endif
