/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPRANDINT_HPP
#define CPRANDINT_HPP

class CpRandInt {
	public:
		CpRandInt() {}
		int operator() (int range);
};

#endif
