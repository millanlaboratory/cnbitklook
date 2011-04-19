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
		printf("[CcLogger::Open] Opening file: %s\n", filename.c_str());
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
	if(CcObject::IsVerbose())
		printf("[CcLogger::Close] Closing file\n");
	this->_semxml.Wait();
	this->_filexml.close();
	this->_semxml.Post();

	CcThread::Stop();
	CcThread::Join();
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
				entry->_caller.c_str(), entry->_message.c_str());
		return;
	}

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
