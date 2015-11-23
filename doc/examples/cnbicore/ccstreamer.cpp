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

#include <cnbicore/CcStreamer.hpp>
#include <stdio.h>
#include <string>
using namespace std;

int main(void) {
	CcStreamer streamer;

	string a("<begin>0</end>");
	string b("<begin>1</end>");
	string c("<begin>2</end>");
	string d("<antani>2</antani>");
	
	/*
	for(int i = 0; i < 10; i++)
		streamer.Append(a);
	
	string buffer;
	for(int i = 0; i < 10 + 1; i++) {
		streamer.Dump();
		if(!streamer.Extract(&buffer, "<begin>", "</end>"))
			cout << "Is empty!" << endl;
	}

	for(int i = 0; i < 10000; i++) {
		streamer.Append(a);
		streamer.Append(b);
		streamer.Append(c);
	}
	cout << "Size: " << streamer.Size() << endl;
	
	while(streamer.Extract(&buffer, "<begin>", "</end>"));
	cout << "Now should be empty" << endl;
	streamer.Dump();
	cout << "Size: " << streamer.Size() << endl;

	string t("<begin></end>");
	streamer.Append(t);
	streamer.Extract(&buffer, "<begin>", "</end>");

	streamer.Append(a);
	streamer.Append(b);
	streamer.Append(c);
	
	for(int i = 0; i < 3 + 1; i++) {
		if(!streamer.Extract(&buffer, "<begin>", "</end>"))
			cout << "Is empty!" << endl;
		else
			cout << "Extracted: " << buffer << endl;
	}
	*/

	streamer.Append(a);
	streamer.Append(b);
	streamer.Append(c);
	streamer.Dump();

	string buffer;
	streamer.Extract(&buffer, "<begin>", "</end>", CcStreamer::Forward);
	streamer.Dump();

	streamer.Extract(&buffer, "<begin>", "</end>", CcStreamer::Reverse);
	streamer.Dump();
	
	streamer.Dump();
	streamer.Append(d);
	streamer.Append(a);
	streamer.Append(b);
	streamer.Append(c);
	streamer.Dump();
	streamer.Extract(&buffer, "<antani>", "</antani>", CcStreamer::Reverse);
	cout << "Buffer is: " << buffer << endl;
	streamer.Dump();

	return 0;
}
