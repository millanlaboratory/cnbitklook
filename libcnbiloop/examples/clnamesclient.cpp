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

#include "ClNamesClient.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcFile.hpp>
#include <iostream>

using namespace std;

int main(void) {
	CcCore::OpenLogger("clclientacq");

	ClNamesClient client;
	if(client.Connect("127.0.0.1:8000") == false) {
		CcLogFatal("Cannot connect to endpoint");
		exit(1);
	}
	
	CcAddress address;
	if(client.Connect()) {
		client.Set("/pippo", "127.0.0.1:12000");
		client.Set("/pippo", "127.0.0.1:12000");
		client.Query("/pippo", &address);
		client.Query("/pluto", &address);
		client.Unset("/pippo");
		client.Unset("/pippo");
		CcTime::Sleep(2000.00f);
	}

	if(client.Connect()) {
		client.Set("/pippo", "127.0.0.1:12000");
		client.Set("/pluto", "127.0.0.1:12001");
		client.Set("/minie", "127.0.0.1:12002");
		
		client.Unset("/pippo");
		client.Unset("/pluto");
		client.Unset("/minie");
		CcTime::Sleep(2000.00f);
	}

	if(client.Connect()) {
		client.Store("config.xml", "Blah blah blah");
		client.Store("config.xml", "Bho bho bho");
		std::cout << "Result: " <<  client.Retrieve("config.xml") << std::endl;
		client.Erase("config.xml");
		client.Erase("config.xml");
		std::cout << "Result: " <<  client.Retrieve("config.xml") << std::endl;
		CcTime::Sleep(2000.00f);
	}

	if(client.Connect()) {
		CcFile file;
		file.Load("/etc/fstab");
		client.Store("fstab", &file);
		file.Clear();
		client.Retrieve("fstab", &file);
		client.Erase("fstab");
	}


	return 0;
}
