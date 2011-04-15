/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

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

#ifndef CCLOGGER_CPP
#define CCLOGGER_CPP

#include "CcLog.hpp"
#include "CcLogger.hpp"
#include "CcTime.hpp"
#include <stdio.h>

CcLogger::CcLogger(void) {
	CcObject::SetName("CcLogger");
	this->_sem.Wait();
	this->_isopen = false;
	this->_sem.Post();
}

CcLogger::~CcLogger(void) {
}

void CcLogger::Open(std::string filename, std::string module,
		CcTermType termtype, CcLogLevel level) {
	
	if(CcObject::IsVerbose())
		printf("[CcLogger::Open] Opening file: %s\n", filename.c_str());
	this->_sem.Wait();
	std::string timestamp;
	CcTime::Datetime(&timestamp);
	this->_file.open(filename.c_str());
	this->_isopen = true;
	this->_termtype = termtype;
	this->_level = level;

	this->_file << "<?xml version='1.0'?>\n";
	this->_file <<	"<!--20100715 M. Tavella <michele.tavella@epfl.ch>-->\n";
	this->_file <<	"<?xml-stylesheet type='text/xsl' href='cclogger.xsl'?>\n";
	this->_file <<	"<CcLogger ";
	this->_file <<	"xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' ";
	this->_file <<	"xmlns:xsd='http://www.w3.org/2001/XMLSchema' timestamp='";
	this->_file << timestamp;
	this->_file <<	"' module='";
	this->_file <<	module;
	this->_file <<	"'>\n";
	this->_file << "</CcLogger>";
	this->_file.flush();
	this->_sem.Post();
}

void CcLogger::Close(void) {
	if(CcObject::IsVerbose())
		printf("[CcLogger::Close] Closing file\n");
	this->_sem.Wait();
	this->_file.close();
	this->_isopen = false;
	this->_sem.Post();
}
bool CcLogger::IsOpen(void) {
	bool result;
	this->_sem.Wait();
	result = this->_isopen;
	this->_sem.Post();
	return result;
}

void CcLogger::AddEntry(CcLogEntry entry) {
	long pos;
	this->_sem.Wait();
	if(this->_isopen) {
		pos = this->_file.tellp();
		this->_file.seekp(pos - 11);
		this->_file << "\t\t";
		this->_file << entry.Serialize();
		this->_file << std::endl;
		this->_file << "</CcLogger>";
		this->_file.flush();
	}
	if(this->_termtype >= CcCore::TerminalEnabled)
		this->DumpEntry(&entry);
	this->_sem.Post();
}

void CcLogger::DumpEntry(CcLogEntry* entry) {
	if(entry->GetLevel() < this->_level)
		return;
	
	if(this->_termtype == CcCore::TerminalDisabled)
		return;
	else if(this->_termtype == CcCore::TerminalEnabled) {
		printf("[%s] %s\n", 
				entry->_caller.c_str(), entry->_message.c_str());
		return;
	}

// \033[1;34m[%s]\033[0m
// \033[1;37m%s 
	else if(this->_termtype == CcCore::TerminalColors) {
		switch(entry->_level) {
			case CcCore::LevelConfig:
				printf("\033[1;37m%s::\033[1;34m[%s]\033[0m\n %s\n", 
						CcCore::GetModulename().c_str(), 
						entry->_caller.c_str(), entry->_message.c_str());
				break;
			case CcCore::LevelException:
				printf("\033[1;37m%s::\033[1;35m[%s]\033[0m\n %s\n", 
						CcCore::GetModulename().c_str(), 
						entry->_caller.c_str(), entry->_message.c_str());
				break;
			case CcCore::LevelDebug:
				printf("\033[1;37m%s::\033[1m[%s]\033[0m\n %s\n", 
						CcCore::GetModulename().c_str(), 
						entry->_caller.c_str(), entry->_message.c_str());
				break;
			case CcCore::LevelInfo:
				printf("\033[1;37m%s::\033[1;32m[%s]\033[0m\n %s\n", 
						CcCore::GetModulename().c_str(), 
						entry->_caller.c_str(), entry->_message.c_str());
				break;
			case CcCore::LevelWarning:
				printf("\033[1;37m%s::\033[1;33m[%s]\033[0m\n %s\n", 
						CcCore::GetModulename().c_str(), 
						entry->_caller.c_str(), entry->_message.c_str());
				break;
			case CcCore::LevelError:
				printf("\033[1;37m%s::\033[0;31m[%s]\033[0m\n %s\n", 
						CcCore::GetModulename().c_str(), 
						entry->_caller.c_str(), entry->_message.c_str());
				break;
			case CcCore::LevelFatal:
				printf("\033[1;37m%s::\033[1;31m[%s]\033[0m\n %s\n", 
						CcCore::GetModulename().c_str(), 
						entry->_caller.c_str(), entry->_message.c_str());
				break;
			default:
				break;
		}
	}
}

#endif
