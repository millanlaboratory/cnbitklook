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

#ifndef CCCORE_CPP
#define CCCORE_CPP

#include "CcCore.hpp"
#include "CcLog.hpp"
#include "CcLogger.hpp"
#include "CcObject.hpp"
#include "CcPtable.hpp"
#include "CcThreadSafe.hpp"
#include "CcTime.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

/* Initialization */
CcCore* CcCore::_instance = NULL;
unsigned int CcCore::_refCount = 0;

/* Declarations */
CcLogger CcCore::logger;
CcThreadSafe<bool> CcCore::receivedAny(false);
CcThreadSafe<bool> CcCore::receivedSIGINT(false);
CcThreadSafe<bool> CcCore::receivedSIGQUIT(false);
CcThreadSafe<bool> CcCore::receivedSIGTERM(false);
CcThreadSafe<bool> CcCore::receivedSIGCHLD(false);

void cccore_sigint(int sig) {
	CcLogFatal("Catched SIGINT");
	CcCore::receivedSIGINT.Set(true);
	CcCore::receivedAny.Set(true);
}

void cccore_sigquit(int sig) {
	CcLogFatal("Catched SIGQUIT");
	CcCore::receivedSIGQUIT.Set(true);
	CcCore::receivedAny.Set(true);
}

void cccore_sigterm(int sig) {
	CcLogFatal("Catched SIGQUIT");
	CcCore::receivedSIGTERM.Set(true);
	CcCore::receivedAny.Set(true);
}

void cccore_sigchild(int sig) {
	CcLogFatal("Catched CHILD");
	CcCore::receivedSIGCHLD.Set(true);
	CcCore::receivedAny.Set(true);
}

CcCore::CcCore(void) {
	/*
	printf("\033[1;31m%s\033[0m %s\n", 
			"[CcCore::CcCore]", 
			"Core functionalities loaded");
	printf(" User: %s\n", CcCore::GetUsername().c_str()); 
	printf(" Home: %s\n", CcCore::GetDirectoryHome().c_str()); 
	printf(" Tmp:  %s\n", CcCore::GetDirectoryTmp().c_str()); 
	printf(" Cwd:  %s\n", CcCore::GetDirectoryCwd().c_str()); 
	*/
}

CcCore::~CcCore(void) {
	CcCore::logger.Close();
	/*
	printf("\033[1;31m%s\033[0m %s\n", 
			"[CcCore::~CcCore]", 
			"Core functionalities unloaded");
	 */
}

CcCore* CcCore::Instance(void) {
	if(CcCore::_instance == NULL)
		CcCore::_instance = new CcCore;
	++CcCore::_refCount;
	return CcCore::_instance;
}

unsigned int CcCore::Refcount(void) {
	return CcCore::_refCount;
}

void CcCore::Release(void) {
	if(--CcCore::_refCount < 1) 
		CcCore::Destroy();
}

void CcCore::Destroy(void) {
	if(CcCore::_instance == NULL) 
		return;
	delete CcCore::_instance;
}
		
void CcCore::Status(void) {
	if(CcCore::_refCount > 0)
		printf("[CcCore::Unload] CcObjects still referenced: %d\n",
				CcCore::Refcount());
}
		
void CcCore::OpenLogger(std::string module, CcTermType termtype,
		CcLogLevel level) {
	std::string timestamp;
	CcTime::Datetime(&timestamp);

	std::string directory = CcCore::GetDirectoryTmp();
	if(mkdir(directory.c_str(), 0755) == -1) 
		if(errno != EEXIST)
			directory.assign("./");

	std::string filename;
	filename.append(directory);
	filename.append(timestamp);
	filename.append("_");
	filename.append(module);
	filename.append(".xml");
	CcCore::logger.Open(filename, module, termtype, level);
	CcLogConfig(std::string("CcLogger is logging in: ").append(filename));
}

std::string CcCore::GetDirectoryTmp(void) {
	std::string directory("/tmp/");
	directory.append("cnbitk-");
	directory.append(CcCore::GetUsername());
	directory.append("/");
	return directory;
}
		
std::string CcCore::GetDirectoryCwd(void) {
	return std::string(getcwd(NULL, 0));
}

std::string CcCore::GetDirectoryHome(void) {
	return std::string(getenv("HOME"));
}

std::string CcCore::GetUsername(void) {
	return std::string(getenv("USER"));
}

void CcCore::CatchSIGINT(void) {
	CcLogConfig("Catching SIGINT");
	(void)signal(SIGINT, cccore_sigint);
}

void CcCore::CatchSIGQUIT(void) {
	CcLogConfig("Catching SIGQUIT");
	(void)signal(SIGQUIT, cccore_sigquit);
}

void CcCore::CatchSIGTERM(void) {
	CcLogConfig("Catching SIGTERM");
	(void)signal(SIGTERM, cccore_sigterm);
}

void CcCore::CatchSIGCHLD(void) {
	CcLogConfig("Catching SIGCHLD");
	(void)signal(SIGCHLD, cccore_sigchild);
}

#endif
