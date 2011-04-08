/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */
#ifndef CPRANDINT_CPP
#define CPRANDINT_CPP

#include "CpRandInt.hpp"
#include <time.h>
#include <stdlib.h>

int  CpRandInt::operator() (int range) {
	srand(time(NULL));
	int result = rand() % range;
	return result;
}

#endif
