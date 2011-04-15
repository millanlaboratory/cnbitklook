/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libxmlconfig library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SmartType.hpp"
#include <string>
#include <iostream>

int main(void) {
	SmartType type(true);

	type.Guess("tana");
	type.Guess("1");
	type.Guess("1.00");
	type.Guess("1.01");
	type.Guess("true");
	type.Guess("false");
	type.Guess("127.0.0.1");

	SmartType type2(false);
	
	type2.Guess("1");
	std::cout << "Got: " << type2.Int() << std::endl;
	
	type2.Guess("1.21");
	std::cout << "Got: " << type2.Float() << std::endl;
	
	type2.Guess("true");
	std::cout << "Got: " << type2.Bool() << std::endl;
	
	try {
		std::cout << "Got: " << type2.String() << std::endl;
	} catch(XMLException e) {
		std::cout << e << std::endl;
	}
	return 0;
}
