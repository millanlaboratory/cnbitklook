/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#define EXAMPLE_NAME "[triggerlpt]"

#include "CpTriggerLPT.hpp"
#include <libcnbicore/CcThread.hpp>
#include <libcnbicore/CcTime.hpp>
#include <libcnbicore/CcException.hpp>
#include <stdio.h>
#include <math.h>
#include <signal.h>

int main(void) {
	CpTriggerLPT *trigger = new CpTriggerLPT(-1, 15, 0);

	try {
		trigger->Open();
	} catch(CcException e) {
		std::cout << e << std::endl;
		return 1;
	}
	
	printf("%s Testing pins\n", EXAMPLE_NAME);
	for(int i = 0; i <= 8; i++) {
		int value = (int)pow(2, i);
		printf(" %d", value);
		trigger->Send(value);
		fflush(stdout);
		CcTime::Sleep(500);
	}
	printf("\n");

	printf("%s Sending all values [0,255]\n", EXAMPLE_NAME);
	for(int i = 0; i < 256; i++) {
		trigger->Send(i);
		CcTime::Sleep(25);
	}
	CcTime::Sleep(1000);
	
	delete trigger;

	printf("%s Bye bye\n", EXAMPLE_NAME);
	return 0;
}
