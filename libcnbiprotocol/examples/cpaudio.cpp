/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#include "CpAudio.hpp"
#include <stdio.h>

int main(void) {

	printf("[main] Setting up devices\n");
	CpAudio* audio1 = CpAudio::Open();
	CpAudio* audio2 = CpAudio::Open();
	
	audio1->Close();
	audio2->Close();

	return 0;
}
