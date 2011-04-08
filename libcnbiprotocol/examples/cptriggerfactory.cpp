/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#include "CpTriggerFactory.hpp"
#define EXAMPLE_NAME "[cptriggerfactory]"
#include <stdio.h>

int main(void) {
	CpTrigger* trigger;
	trigger = CpTriggerFactory::Find();
	trigger->Send(1);
	delete trigger;
}
