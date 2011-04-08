/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#ifndef CPTRIGGERBEEP_CPP
#define CPTRIGGERBEEP_CPP

#include "CpTriggerBeep.hpp"
#include <libcnbicore/CcException.hpp>
#include <stdio.h>

CpTriggerBeep::CpTriggerBeep(void) {
	CcObject::SetName("CpTriggerBeep");
	this->_open = false;
}

CpTriggerBeep::~CpTriggerBeep(void) {
}

void CpTriggerBeep::Open(void) {
	this->_open = true;
}

void CpTriggerBeep::Close(void) {
	this->_open = false;
}

void CpTriggerBeep::Send(unsigned char data) {
	if(this->_open == false)
		throw CcException("Beeper is not open", __PRETTY_FUNCTION__);
	printf("\a");
	fflush(stdout);
}

#endif
