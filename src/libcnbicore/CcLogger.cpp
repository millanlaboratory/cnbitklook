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

#define	DARK_WHITE    	"\033[1;37m"
#define DARK_BLUE		"\033[1;34m"
#define LIGHT_PURPLE 	"\033[1;35m"
#define LIGHT_WHITE  	"\033[1m"
#define LIGHT_GREEN		"\033[1;32m"
#define LIGHT_YELLOW 	"\033[1;33m"
#define LIGHT_RED		"\033[0;31m"
#define DARK_RED 		"\033[1;31m"
#define NOCOLOR			"\033[0m"

CcLogger::CcLogger(bool syncdump, double writems) {
	CcObject::SetName("CcLogger");
	this->_listW = &this->_list1;
	this->_listR = &this->_list2;
	this->_writems = writems;
	this->_syncdump = syncdump;
}

CcLogger::~CcLogger(void) {
}

void CcLogger::Open(const std::string& filename, const std::string& module,
		const CcTermType termtype, const CcLogLevel level) {
	
	if(CcObject::IsVerbose())
		printf("[CcLogger::Open] Opening file (sync=%d, wait=%f): %s\n", 
				this->_syncdump, this->_writems, filename.c_str());
	this->_semxml.Wait();
	std::string timestamp;
	CcTime::Datetime(&timestamp);
	this->_filexml.open(filename.c_str());
	this->_termtype = termtype;
	this->_level = level;

	this->_filexml << "<?xml version='1.0'?>\n";
	this->_filexml <<	"<!--20100715 M. Tavella <michele.tavella@epfl.ch>-->\n";
	this->_filexml <<	"<?xml-stylesheet type='text/xsl' href='cclogger.xsl'?>\n";
	this->_filexml <<	"<CcLogger ";
	this->_filexml <<	"xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' ";
	this->_filexml <<	"xmlns:xsd='http://www.w3.org/2001/XMLSchema' timestamp='";
	this->_filexml << timestamp;
	this->_filexml <<	"' module='";
	this->_filexml <<	module;
	this->_filexml <<	"'>\n";
	this->_filexml << "</CcLogger>";
	this->_filexml.flush();
	this->_semxml.Post();

	CcThread::Start();
}

void CcLogger::Close(void) {
	if(CcThread::IsRunning()) {
		CcThread::Stop();
		CcThread::Join();
	}
	
	this->_semxml.Wait();
	if(this->_filexml.is_open())
		this->_filexml.close();
	this->_semxml.Post();
	
	if(CcObject::IsVerbose())
		printf("[CcLogger::Close] File is close and thread is down\n");
}

bool CcLogger::IsOpen(void) {
	bool result;
	this->_semxml.Wait();
	result = this->_filexml.is_open();
	this->_semxml.Post();
	return result;
}

void CcLogger::AddEntry(CcLogEntry entry) {
	this->_semlist.Wait();
	this->_listW->push_back(entry);
	if(this->_syncdump == true)
		this->DumpEntry(&entry);
	this->_semlist.Post();
}

void CcLogger::WriteEntry(CcLogEntry* entry) {
	if(this->_filexml.is_open() == false)
		return;

	long pos = this->_filexml.tellp();
	this->_filexml.seekp(pos - 11);
	this->_filexml << "\t\t";
	this->_filexml << entry->Serialize();
	this->_filexml << std::endl;
	this->_filexml << "</CcLogger>";
	this->_filexml.flush();
}

void CcLogger::DumpEntry(CcLogEntry* entry) {
	if(this->_termtype < CcCore::TerminalEnabled)
		return;
	if(entry->GetLevel() < this->_level)
		return;
	
	if(this->_termtype == CcCore::TerminalDisabled)
		return;
	else if(this->_termtype == CcCore::TerminalEnabled) {
		printf("[%s] %s\n", 
				entry->_timestamp.c_str(), entry->_message.c_str());
		return;
	}
/*
#define	DARK_WHITE    	"\033[1;37m"
#define DARK_BLUE		"\033[1;34m"
#define LIGHT_PURPLE 	"\033[1;35m"
#define LIGHT_WHITE  	"\033[1m"
#define LIGHT_GREEN		"\033[1;32m"
#define LIGHT_YELLOW 	"\033[1;33m"
#define LIGHT_RED		"\033[0;31m"
#define DARK_RED 		"\033[1;31m"
#define NOCOLOR			"\033[0m"
	*/

	else if(this->_termtype == CcCore::TerminalColors) {
		const char* color = NULL;
		std::string level;
		switch(entry->_level) {
			case CcCore::LevelConfig:
				level.assign("Cfg");
				color = DARK_BLUE;
				break;
			case CcCore::LevelException:
				level.assign("Exc");
				color = LIGHT_PURPLE;
				break;
			case CcCore::LevelDebug:
				level.assign("Dbg");
				color = LIGHT_WHITE;
				break;
			case CcCore::LevelInfo:
				level.assign("Inf");
				color = LIGHT_GREEN;
				break;
			case CcCore::LevelWarning:
				level.assign("Wrg");
				color = LIGHT_YELLOW;
				break;
			case CcCore::LevelError:
				level.assign("Err");
				color = LIGHT_RED;
				break;
			case CcCore::LevelFatal:
				level.assign("Ftl");
				color = DARK_RED;
				break;
			default:
				color = NOCOLOR;
				break;
		}

		printf("%s[%s%s %s%-3.3s %s]%s %s\n", 
				(const char*)color, 
				DARK_WHITE, 
				CcCore::GetModulename().c_str(), 
				color, 
				level.c_str(),
				entry->_timestamp.c_str(), 
				NOCOLOR,
				entry->_message.c_str());
		/*
		printf("%s[%s%s %s%-3.3s]%s %s\n", 
				(const char*)color, 
				DARK_WHITE, 
				CcCore::GetModulename().c_str(), 
				color, 
				level.c_str(),
				NOCOLOR,
				entry->_message.c_str());
		 */
	}
}

void CcLogger::Main(void) {
	std::list<CcLogEntry>::iterator it;
	CcLogEntry* entry;

	while(CcThread::IsRunning()) {
		CcTime::Sleep(this->_writems);

		this->_semlist.Wait();
		this->SwitchList();
		this->_semlist.Post();
			
		this->_semxml.Wait();
		for (it = this->_listR->begin(); it != this->_listR->end(); it++) {
			entry = &(*it);
			if(this->_syncdump == false)
				this->DumpEntry(entry);
			this->WriteEntry(entry);
		}
		this->_semxml.Post();
		this->_listR->clear();
	}
}
		
void CcLogger::SwitchList(void) {
	if(this->_listR == &this->_list1) {
		this->_listR = &this->_list2;
		this->_listW = &this->_list1;
	} else {
		this->_listR = &this->_list1;
		this->_listW = &this->_list2;
	}
}

#endif
