/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#include "CpTrials.hpp"
#include <iostream>

int main(void) {
	
	CpTrials trials;
	trials.AddClassLabel(0, 5);
	trials.AddClassLabel(2, 5);
	trials.AddClassLabel(1, 5);
	trials.AddClassLabel(3, 5);
	trials.AddClassLabel(4, 5);
	trials.AddClassLabel(5, 5);
	trials.AddClassLabel(6, 5);
	trials.AddClassLabel(7, 5);
	trials.AddClassLabel(8, 10);
	trials.Create();
	trials.Dump();
	trials.Shuffle();
	trials.Dump();
	trials.IncreaseClassLabel(0, 5);
	trials.Create();
	trials.Shuffle();
	trials.Dump();
	
	CpTrials trialsE;
	std::cout << trialsE.Create() << std::endl;

	return 0;
}
