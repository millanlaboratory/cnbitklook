/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#include "CpTriggerBeep.hpp"
#define EXAMPLE_NAME "[cptriggerbeep]"
#include <stdio.h>

int main(void) {
	CpTriggerBeep beeper;
	beeper.Open();

	for(int j = 0; j < 4; j++) {
		printf("%s Press enter to send trigger ", EXAMPLE_NAME);
		getchar();
		beeper.Send();
	}
	beeper.Close();
}
