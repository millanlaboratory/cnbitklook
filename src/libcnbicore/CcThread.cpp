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

#ifndef CCTHREAD_CPP
#define CCTHREAD_CPP

#include "CcThread.hpp"
#include "CcThreadrunner.hpp"

CcThread::CcThread(void) {
	CcObject::SetName("CcThread");
	this->_running = false;
}

CcThread::~CcThread(void) {
	this->Stop();
	this->Join();
}

void CcThread::Start(void) {
	this->pBeforeStart();
	this->_semstatus.Wait();
	if(this->_running == true) {
		this->_semstatus.Post();
		return;
	}

	this->_semthread.Wait();
	int status = 0;
	status = pthread_create(&this->_thread, NULL, CcThreadrunner, (void *)this);
	this->_running = (status == 0);
	this->_started = (status == 0);
	this->_semthread.Post();
	this->_semstatus.Post();
	this->pAfterStart();
}

void CcThread::Stop(void) {
	this->pBeforeStop();
	this->_semstatus.Wait();
	this->_running = false;
	this->_semstatus.Post();
	this->pAfterStop();
}

int CcThread::Join(void) {
	int status = -1;
	
	this->_semthread.Wait();
	bool started = this->_started;
	this->_semthread.Post();

	if(started == true) {
		status = pthread_join(this->_thread, NULL);
		this->_started = (status == 0);
	}
	return status;
}
		
bool CcThread::IsRunning(void) { 	
	this->_semstatus.Wait();
	bool status = this->_running;
	this->_semstatus.Post();

	return status;
}
		
#endif
