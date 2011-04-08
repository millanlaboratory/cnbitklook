/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */
#include <libcnbicore/CcCore.hpp>
#include <libcnbicore/CcException.hpp>
#include "CpTriggerLPT.hpp"
#include "CpTriggerBeep.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(void) {
	printf("[cp_triggersend] Allocating CcTrigger objects\n");
	CcTriggerLPT* lpt = new CcTriggerLPT(-1, 15, 0);
	CcTriggerBeep* beep = new CcTriggerBeep();

	try {
		lpt->Open();
	} catch(CcException e) {
		cout << e << endl;
		delete lpt;
		lpt = NULL;
	}

	try {
		beep->Open();
	} catch(CcException e) {
		cout << e << endl;
		delete beep;
		beep = NULL;
	}

	printf("[cp_triggersend] Entering main loop\n");
	string input;
	while(true) {
		printf(">> ");
		getline(cin, input);
	
		if(input.compare("quit") == 0)
			break;
		if(input.compare("q") == 0)
			break;
		
		if(lpt)
			lpt->Send(1);
		if(lpt)
			beep->Send();
	}
	
	if(lpt)
		delete lpt;
	if(beep)
		delete beep;
	return 0;
}
