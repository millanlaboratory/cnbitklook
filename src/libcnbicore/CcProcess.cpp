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

#ifndef CCPROCESS_CPP 
#define CCPROCESS_CPP 

#include "CcProcess.hpp" 
#include "CcPtable.hpp" 
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void cphandlesigpipe(int signum) {
	CcLogWarning("Received SIGPIPE");
	pid_t pid = wait(NULL);
	if(CcPtable::Has(pid))
		CcPtable::SetDead(pid);
}

void cphandlesigchild(int signum) {
	CcLogWarning("Received SIGCHLD");
	pid_t pid = wait(NULL);
	if(CcPtable::Has(pid))
		CcPtable::SetDead(pid);
}

void cpcatchsigchild(void) {
	signal(SIGCHLD, cphandlesigchild);
}
void cpcatchsigpipe(void) {
	signal(SIGPIPE, cphandlesigpipe);
}

CcProcess::CcProcess(const std::string& cmd, bool write, bool read) {
	CcObject::SetName("CcProcess");
	this->_cmd.assign(cmd);
	this->_canread = read;
	this->_canwrite = write;
	cpcatchsigpipe();
	cpcatchsigchild();
}

CcProcess::~CcProcess(void) {
	if(CcPtable::Has(this->_pid) == true)
		CcPtable::Remove(this->_pid);
}
		
pid_t CcProcess::Popen(int* input, int* output) {
	int p_stdin[2], p_stdout[2];
    pid_t pid;

    if(pipe(p_stdin) != 0 || pipe(p_stdout) != 0)
        return -1;

    pid = fork();
    if (pid < 0) {
        return pid;
	} else if(pid == 0) {
		if(input != NULL) {
			CcLogDebug("Managing stdinput");
			close(p_stdin[CcProcess::ProcessWrite]);
			dup2(p_stdin[CcProcess::ProcessRead], CcProcess::ProcessRead);
		}
		if(output != NULL) {
			CcLogDebug("Managing stdoutput");
			close(p_stdout[CcProcess::ProcessRead]);
			dup2(p_stdout[CcProcess::ProcessWrite], CcProcess::ProcessWrite);
		}
		this->Exec();
        perror("execl");
        exit(1);
    }

    if (input == NULL)
        close(p_stdin[CcProcess::ProcessWrite]);
    else
        *input = p_stdin[CcProcess::ProcessWrite];

    if (output == NULL)
        close(p_stdout[CcProcess::ProcessRead]);
    else
        *output = p_stdout[CcProcess::ProcessRead];

    return pid;
}

void CcProcess::Exec(void) {
	execl("/bin/sh", "sh", "-c", this->_cmd.c_str(), NULL);
}

pid_t CcProcess::Fork(void) {
	int* ptrWrite = NULL;
	int* ptrRead = NULL;
	
	if(this->_canread)
		ptrRead = &this->_fidRead;
	if(this->_canwrite)
		ptrWrite = &this->_fidWrite;
	this->_pid = this->Popen(ptrWrite, ptrRead);
	if(this->_pid <= 0) {
		CcLogFatal("Cannot fork process");
		return this->_pid;
	}

	CcLogDebugS("Forked: " << this->_pid);
	if(CcPtable::Has(this->_pid) == false)
		CcPtable::Add(this->_pid);
	else
		CcPtable::Replace(this->_pid);
	return this->_pid;
}

int CcProcess::Terminate(void) {
	int status = kill(this->_pid, SIGTERM);
	if(status == 0) {
		CcLogDebugS("Terminated: " << this->_pid);
		if(CcPtable::Has(this->_pid) == true)
			CcPtable::SetDead(this->_pid);
	} else {
		CcLogWarningS("Can not terminate: " << this->_pid);
	}
	return status;
}

int CcProcess::Kill(void) {
	int status = kill(this->_pid, SIGKILL);
	if(status == 0) {
		CcLogDebugS("Killed: " << this->_pid);
		if(CcPtable::Has(this->_pid) == true)
			CcPtable::SetDead(this->_pid);
	} else {
		CcLogWarningS("Can not kill: " << this->_pid);
	}
	return status;
}

int CcProcess::Write(const std::string& buffer) {
	if(this->_canwrite == false)
		CcThrow("Child cannot accept input");
	return write(this->_fidWrite, buffer.c_str(), buffer.size());
}

int CcProcess::Write(const char* buffer) {
	if(this->_canwrite == false)
		CcThrow("Child cannot accept input");
	return write(this->_fidWrite, buffer, strlen(buffer));
}

bool CcProcess::IsAlive(void) {
	return CcPtable::IsAlive(this->_pid);
}

int CcProcess::Read(char* buffer, size_t count) {
	return read(this->_fidRead, buffer, count);
}

#endif
