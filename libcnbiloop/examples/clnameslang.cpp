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

#include "ClNamesLang.hpp"
#include <iostream>

using namespace std;

int main(void) {

	ClNamesLang names;
	std::string name, address;
	
	name.clear();
	address.clear();
	cout << names.Query("/antani") << endl;
	cout << names.IsQuery(names.message->buffer, &name) << endl;
	cout << "--> Name:    " << name << endl;
	
	name.clear();
	address.clear();
	cout << names.Reply("127.0.0.1:8000") << endl;
	cout << names.IsReply(names.message->buffer, &address) << endl;
	cout << "--> Address: " << address << endl;
	
	name.clear();
	address.clear();
	cout << names.Set("/antani", "127.0.0.1:8000") << endl;
	cout << names.IsSet(names.message->buffer, &name, &address) << endl;
	cout << "--> Name:    " << name << endl;
	cout << "--> Address: " << address << endl;
	
	name.clear();
	address.clear();
	cout << names.Unset("/antani") << endl;
	cout << names.IsUnset(names.message->buffer, &name) << endl;
	cout << "--> Name:    " << name << endl;
	
	int id;
	cout << names.Error(666) << endl;
	cout << names.IsError(names.message->buffer, &id) << endl;
	cout << "--> ID:    " << id << endl;
	
	cout << names.Ok() << endl;
	cout << names.IsOk(names.message->buffer) << endl;


	return 0;
}
