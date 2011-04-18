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

#include "CCfgXMLConfig.hpp"
#include <rapidxml.hpp>
#include <cstdio>
#include <iostream>

using namespace rapidxml;

CCfgXMLConfig::CCfgXMLConfig(const std::string& rootname) {
	this->_nRoot = NULL;
	this->_nLeaf = NULL;
	this->_nLeaf = NULL;
	this->_rootname.assign(rootname);
}

CCfgXMLConfig::~CCfgXMLConfig(void) {
}

void CCfgXMLConfig::ImportFileEx(const std::string& filename) {
	switch(this->_document.ImportFile(filename)) {
		case XMLDocument::FileInputError:
			throw XMLException("File Input Error", __PRETTY_FUNCTION__);
		case XMLDocument::BufferOverflow:
			throw XMLException("Buffer Overflow", __PRETTY_FUNCTION__);
		case XMLDocument::ParsingError:
			throw XMLException("Parsing Error", __PRETTY_FUNCTION__);
	}

	this->SearchRoot();
	this->CheckRootEx();
}

void CCfgXMLConfig::ImportBufferEx(const std::string& filename) {
	switch(this->_document.ImportBuffer(filename)) {
		case XMLDocument::FileInputError:
			throw XMLException("File Input Error", __PRETTY_FUNCTION__);
		case XMLDocument::BufferOverflow:
			throw XMLException("Buffer Overflow", __PRETTY_FUNCTION__);
		case XMLDocument::ParsingError:
			throw XMLException("Parsing Error", __PRETTY_FUNCTION__);
	}
	this->SearchRoot();
	this->CheckRootEx();
}
	
void CCfgXMLConfig::SearchRoot(void) {
	this->_nRoot = this->_document.doc.first_node(this->_rootname.c_str());
}

void CCfgXMLConfig::CheckRootEx(void) {
	if(this->_nRoot == NULL)
		throw XMLException("Root node not found", __PRETTY_FUNCTION__);
}

void CCfgXMLConfig::CheckBranchEx(void) {
	if(this->_nBranch == NULL)
		throw XMLException("Branch node not found", __PRETTY_FUNCTION__);
}

void CCfgXMLConfig::CheckLeafEx(void) {
	if(this->_nLeaf == NULL)
		throw XMLException("Leaf node not found", __PRETTY_FUNCTION__);
}

CCfgXMLConfig* CCfgXMLConfig::RootEx(void) {
	this->CheckRootEx();
	this->_nLeaf = this->_nRoot;
	this->_nBranch = this->_nRoot;
	return this;
}

CCfgXMLConfig* CCfgXMLConfig::GoEx(const std::string& blockname) {
	XMLNode node = this->_nLeaf->first_node(blockname.c_str());
	if(node == NULL) {
		std::string info;
		info += "Node '";
		info += this->_nLeaf->name();
		info += "/";
		info += blockname;
		info += "' not found";
		throw XMLException(info, __PRETTY_FUNCTION__);
	} else
		this->_nLeaf = node;

	return this;
}

CCfgXMLConfig* CCfgXMLConfig::QuickEx(const std::string& blockpath) {
	std::vector<std::string> path;
	this->Tokenize(blockpath, path);
	if(path.size() == 0) {
		std::string info;
		info.append("Zero tokens found in: ");
		info.append(blockpath);
		throw XMLException(info, __PRETTY_FUNCTION__);
	}
		
	for(unsigned int i = 0; i < path.size(); i++)
		this->GoEx(path[i]);
	return this;
}

std::string CCfgXMLConfig::GetRawEx(void) {
	this->CheckLeafEx();
	std::string value(this->_nLeaf->value());
	return value;
}

CCfgXMLConfig* CCfgXMLConfig::GetRawEx(std::string* value) {
	this->CheckLeafEx();
	value->assign(this->_nLeaf->value());
	return this;
}

XMLType CCfgXMLConfig::GetEx(void) {
	this->CheckLeafEx();
	XMLNode nOld = this->_nLeaf;

	XMLType value((const char*)this->_nLeaf->value());

	XMLAttr nId = this->_nLeaf->first_attribute("id");
	if(nId != NULL)
		value.SetId(nId->value());
	
	XMLAttr nKey = this->_nLeaf->first_attribute("key");
	if(nKey != NULL)
		value.SetKey(nKey->value());

	this->_nLeaf = nOld;
	return value;
}
		
CCfgXMLConfig* CCfgXMLConfig::GetEx(XMLType* value) {
	this->CheckLeafEx();
	value->Guess((const char*)this->_nLeaf->value());
	return this;
}
		
std::string CCfgXMLConfig::GetAttrEx(const std::string& name) {
	this->CheckLeafEx();
	
	std::string attr;
	XMLAttr nId = this->_nLeaf->first_attribute(name.c_str());
	if(nId != NULL)
		attr.assign(nId->value());
	return attr;
}

CCfgXMLConfig* CCfgXMLConfig::SetBranch(void) {
	this->_nBranch = this->_nLeaf;
	return this;
}

CCfgXMLConfig* CCfgXMLConfig::BranchEx(void) {
	this->CheckBranchEx();
	this->_nLeaf = this->_nBranch;
	return this;
}

XMLNode CCfgXMLConfig::Leaf(void) {
	return this->_nLeaf;
}

XMLNode CCfgXMLConfig::Child(void) {
	XMLNode node = this->_nLeaf->first_node();
	if(node != NULL)
		this->_nLeaf = node;
	return node;
}

XMLNode CCfgXMLConfig::NextSibling(void) {
	XMLNode node = this->_nLeaf->next_sibling();
	if(node != NULL)
		this->_nLeaf = node;
	return node;
}

void CCfgXMLConfig::Dump(void) {
	printf("[CCfgXMLConfig::Dump] Nodes point to:\n");
	printf(" Root:   %p, '%s', '%s'\n", this->_nRoot, 
			this->_nRoot->name(), this->_nRoot->value());
	printf(" Branch: %p, '%s', '%s'\n", this->_nBranch, 
			this->_nBranch->name(), this->_nBranch->value());
	printf(" Leaf:   %p, '%s', '%s'\n", this->_nLeaf, 
			this->_nLeaf->name(), this->_nLeaf->value());
}

std::string CCfgXMLConfig::QuickStringEx(const std::string& str) {
	return this->QuickEx(str)->GetEx().String();
}

bool CCfgXMLConfig::QuickBoolEx(const std::string& str) {
	return this->QuickEx(str)->GetEx().Bool();
}

float CCfgXMLConfig::QuickFloatEx(const std::string& str) {
	return this->QuickEx(str)->GetEx().Float();
}

int CCfgXMLConfig::QuickIntEx(const std::string& str) {
	return this->QuickEx(str)->GetEx().Int();
}

unsigned int CCfgXMLConfig::QuickHexIntEx(const std::string& str) {
	return this->QuickEx(str)->GetEx().HexInt();
}

void CCfgXMLConfig::Tokenize(const std::string& str, 
		std::vector<std::string>& tokens, const std::string& delimiters) {
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while(std::string::npos != pos || std::string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}
