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

#include "ClAcqClient.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

int main(void) {
	CcCore::OpenLogger("clclientacq");

	ClAcqClient client;
	if(client.Connect("127.0.0.1:9000") == false) {
		CcLogFatal("Cannot connect to endpoint");
		exit(1);
	}

	while(client.Connect()) {
		client.OpenXDF("test.xdf", "test.log", "antani=10");
		client.OpenXDF("test2.xdf", "test2.log", "antani=20");
		client.CloseXDF();
		CcTime::Sleep(2000.00f);
	}
	return 0;
}
