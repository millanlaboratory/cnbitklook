/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#ifndef CCTRIGGERFACTORY_CPP
#define CCTRIGGERFACTORY_CPP

#include <libcnbicore/CcException.hpp>
#include "CpTriggerFactory.hpp"
#include <stdio.h>

CpTrigger* CpTriggerFactory::Find(void) {
	CpTrigger* trigger = NULL;

	try {
		trigger = new CpTriggerLPT(-1, 15, 0);
		trigger->Open();
		printf("[CpTriggerFactory::Open] LPT capabilities detected\n");
	} catch(CcException e) {
		printf("[CpTriggerFactory::Open] Limited LPT capabilities: using beeper\n");
		delete trigger;
		trigger = new CpTriggerBeep();
		trigger->Open();
	}
	return trigger;
}

#endif
