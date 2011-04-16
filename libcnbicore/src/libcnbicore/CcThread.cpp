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
	this->Running(false);
	this->Stopping(false);
}

CcThread::~CcThread(void) {
}

void CcThread::Start(void) {
	this->pBeforeStart();
	if(this->IsRunning())
		return;

	int status = 0;
	this->_mutex.Lock();
	status = pthread_create(&this->_thread, NULL, CcThreadrunner, (void *)this);
	this->_running = (status == 0);
	this->_mutex.Release();
	this->pAfterStart();
}

void CcThread::Stop(void) {
	this->pBeforeStop();
	this->Running(false);
	this->Stopping(true);
	this->pAfterStop();
}

void CcThread::Join(void) {
	pthread_join(this->_thread, NULL);
}
		
long CcThread::GetId(void) {
	return (long)this->_thread;
}

bool CcThread::IsRunning(void) { 	
	this->_mutex.Lock();
	bool status = this->_running;
	this->_mutex.Release();

	return status;
}
		
bool CcThread::IsStopping(void) {
	this->_mutex.Lock();
	bool status = this->_stopping;
	this->_mutex.Release();

	return status;
}

void CcThread::Running(bool status) {
	this->_mutex.Lock();
	this->_running = status;
	this->_mutex.Release();
}

void CcThread::Stopping(bool status) {
	this->_mutex.Lock();
	this->_stopping = status;
	this->_mutex.Release();
}

#endif
