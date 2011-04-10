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
		
	CcFile file;
	file.Load("/home/mtavella/Desktop/culo.txt");
	std::string fileb, fileb2;
	file.Get(&fileb);

	/*
	char cache[1048576];
	char cache2[1048576];
	int w = sprintf(cache, "%s", fileb.c_str());
	//cout << sscanf(cache, "%s", cache2) << endl;
	strncpy(cache2, cache, fileb.size());
	fileb2.assign(cache2);
	
	cout << file.Size() << endl;
	cout << fileb.size() << endl;
	cout << w << endl;
	cout << fileb2.size() << endl;
	//cout << fileb2 << endl;

	return 0;
	*/

	client.StoreFile("culo", "/home/mtavella/Desktop/culo.txt");
	client.RetrieveFile("culo", "group");
	client.Erase("culo");

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
	
		client.StoreFile("group", "/etc/group");
		client.RetrieveFile("group", "group");
		client.Erase("group");
	}

	return 0;
}
