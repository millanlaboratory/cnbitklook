/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ClTobiIc.hpp"
#include <iostream>

using namespace std;

int main(void) {
	CcCore::OpenLogger("cltobiic");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	ClTobiIc ic;
	ic.Open("9500", "/feedback0");

	CcTime::Sleep(10000.00f);
	ic.Close();

	return 0;
}
