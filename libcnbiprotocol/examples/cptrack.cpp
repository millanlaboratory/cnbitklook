/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#include "CpTrack.hpp"
#include <iostream>

int main(void) {
	CpTrack track("1N 1R 1N 2L 1N 1L 3N 2R 1N 1R 1L 1N");
	for(unsigned int i = 0; i < track.Size(); i++) 
		std::cout << i << ") " << track.Get(i) << std::endl;
	
	track.Set("cow milk chicken egg");
	for(unsigned int i = 0; i < track.Size(); i++) 
		std::cout << i << ") " << track.Get(i) << std::endl;
		
	std::cout << "e) " << track.Get(100) << std::endl;

	return 0;
}
