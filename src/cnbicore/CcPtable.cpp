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

#ifndef CCPTABLE_CPP 
#define CCPTABLE_CPP 

#include "CcPtable.hpp" 
#include "CcBasic.hpp" 
#include <string> 

/* Initialization */
CcPtable* CcPtable::_instance = NULL;
unsigned int CcPtable::_refCount = 0;

/* Declarations */
std::map<pid_t, bool> CcPtable::_table;
CcSemaphore CcPtable::_semlock;

CcPtable::CcPtable(void) {
	CcLogDebug("Process table loaded");
}

CcPtable::~CcPtable(void) {
	CcLogDebug("Process table unloaded");
}

CcPtable* CcPtable::Instance(void) {
	if(CcPtable::_instance == NULL) {
		CcPtable::_instance = new CcPtable;
	}
	++CcPtable::_refCount;
	return CcPtable::_instance;
}

void CcPtable::Release(void) {
	if(--CcPtable::_refCount < 1) 
		CcPtable::Destroy();
}

unsigned int CcPtable::Refcount(void) {
	return CcPtable::_refCount;
}

void CcPtable::Destroy(void) {
	if(CcPtable::_instance != NULL) 
		delete CcPtable::_instance;
}

void CcPtable::Add(pid_t pid) {
	CcPtable::_semlock.Wait();
	if(CcPtable::_table.count(pid) == 1) {
		CcPtable::_semlock.Post();
		CcThrow("Process already in Ptable");
	}
	
	CcLogDebug("Adding: ");
	CcPtable::_table[pid] = true;
	CcPtable::_semlock.Post();
}

void CcPtable::Replace(pid_t pid) {
	CcPtable::_semlock.Wait();
	if(CcPtable::_table.count(pid) == 0) {
		CcPtable::_semlock.Post();
		CcThrow("Process not in Ptable");
	}
	
	CcLogDebug("Replacing: ");
	CcPtable::_table[pid] = true;
	CcPtable::_semlock.Post();
}

void CcPtable::Remove(pid_t pid) {
	CcPtable::_semlock.Wait();
	if(CcPtable::_table.count(pid) == 0) {
		CcPtable::_semlock.Post();
		CcThrow("Process not in Ptable");
	}

	std::map<pid_t, bool>::iterator it;
	it = CcPtable::_table.find(pid);
	CcPtable::_table.erase(it);
	CcPtable::_semlock.Post();
}

bool CcPtable::Has(pid_t pid) {
	CcPtable::_semlock.Wait();
	bool status = (CcPtable::_table.count(pid) == 1);
	CcPtable::_semlock.Post();
	return status;
}

unsigned int CcPtable::Size(void) {
	unsigned int size = 0;
	CcPtable::_semlock.Wait();
	size = CcPtable::_table.size();
	CcPtable::_semlock.Post();
	return size;
}

void CcPtable::SetDead(pid_t pid) {
	CcPtable::_semlock.Wait();
	if(CcPtable::_table.count(pid) == 0) {
		CcPtable::_semlock.Post();
		CcThrow("Process not in Ptable");
	}

	CcLogDebug("Dead: ");
	CcPtable::_table[pid] = false;
	CcPtable::_semlock.Post();
}

bool CcPtable::IsDead(pid_t pid) {
	CcPtable::_semlock.Wait();
	if(CcPtable::_table.count(pid) != 1) {
		CcPtable::_semlock.Post();
		CcThrow("Process not in Ptable");
	}
		
	bool status = CcPtable::_table[pid];
	CcPtable::_semlock.Post();
	return !status;
}

bool CcPtable::IsAlive(pid_t pid) {
	CcPtable::_semlock.Wait();
	if(CcPtable::_table.count(pid) != 1) {
		CcPtable::_semlock.Post();
		CcThrow("Process not in Ptable");
	}
		
	bool status = CcPtable::_table[pid];
	CcPtable::_semlock.Post();
	return status;
}
		
void CcPtable::Dump(void) {
	CcPtable::_semlock.Wait();
	if(CcPtable::Refcount() < 0) {
		CcPtable::_semlock.Post();
		return;
	}
	std::map<pid_t, bool>::iterator it;
	pid_t pid;
	bool status;
	printf("[CcPtable::Dump] Process list:\n");
	printf("  PID   Status\n");
	for(it = CcPtable::_table.begin(); it != CcPtable::_table.end(); it++) {
		pid = (*it).first;
		status = (*it).second;
		printf("%5d   %s\n", pid, status ? "Alive" : "Dead");
	}
	CcPtable::_semlock.Post();
}
#endif
