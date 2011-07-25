/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cnbiloop/ClNmsLang.hpp>
#include <iostream>

using namespace std;

int main(void) {

	ClNmsLang names;
	std::string name, address, content;
	
	name.clear();
	address.clear();
	cout << names.Query("/antani") << endl;
	cout << names.IsQuery(names.message->buffer, &name) << endl;
	cout << "--> Name:    " << name << endl;
	cout << "--> Valid:   " << names.CheckName("/antani") << endl;
	cout << "--> Valid:   " << names.CheckName("antani") << endl;
	
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
	
	name.clear();
	content.clear();
	cout << names.Store("config.xml", "<hello>world</hello>") << endl;
	cout << names.IsStore(names.message->buffer, &name, &content) << endl;
	cout << "--> Name:    " << name << endl;
	cout << "--> Content: " << content << endl;
	
	name.clear();
	cout << names.Retrieve("config.xml") << endl;
	cout << names.IsRetrieve(names.message->buffer, &name) << endl;
	cout << "--> Name:    " << name << endl;
	
	name.clear();
	cout << names.Erase("config.xml") << endl;
	cout << names.IsErase(names.message->buffer, &name) << endl;
	cout << "--> Name:    " << name << endl;
	
	content.clear();
	cout << names.Dispatch("<hello>world</hello>") << endl;
	cout << names.IsDispatch(names.message->buffer, &content) << endl;
	cout << "--> Content: " << content << endl;

	name.clear();
	address.clear();

	return 0;
}
