/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#ifndef CPTRIGGERLPT_CPP
#define CPTRIGGERLPT_CPP

#include "CpTriggerLPT.hpp"
#include <libcnbicore/CcException.hpp>

CpTriggerLPT::CpTriggerLPT(int port, unsigned int dt, unsigned char level) {
	CcObject::SetName("CpTriggerLPT");
	this->_port = port;
	this->_dt = dt;
	this->_level = level;
	this->_open = false;
}

CpTriggerLPT::~CpTriggerLPT(void) { 
	this->Close();
}

void CpTriggerLPT::Open(void) { 
	this->_trigger = OpenLPTTrigger(this->_level, this->_dt, this->_port);
	if(this->_trigger == NULL)
		throw CcException("Cannot open LPT port", __PRETTY_FUNCTION__);
	this->_open = true;
}

void CpTriggerLPT::Close(void) { 
	CloseLPTTrigger(this->_trigger);
	this->_open = false;
}

void CpTriggerLPT::Send(CpTriggerByte data) { 
	if(this->_open == false)
		throw CcException("LPT port is not open", __PRETTY_FUNCTION__);
	SignalTrigger(this->_trigger, data);
}

#endif
