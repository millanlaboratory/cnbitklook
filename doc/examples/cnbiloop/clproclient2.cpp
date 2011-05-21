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

#include <cnbiloop/ClProClient.hpp>
#include <cnbicore/CcBasic.hpp>
#include <iostream>

using namespace std;

int main(void) {
	CcCore::OpenLogger("clclientacq");

	ClProClient client;
	if(client.Connect("127.0.0.1:9100") == false) {
		CcLogFatal("Cannot connect to endpoint");
		CcCore::Exit(1);
	}

	int pid0 = 0, pid1 = 1;
	if(client.ForkAndCheck(&pid0) != ClProLang::Successful)
		return 1;
	if(client.ForkAndCheck(&pid1) != ClProLang::Successful)
		return 1;
	cout << "Spawned " << pid0 << endl;
	cout << "Spawned " << pid1 << endl;

	client.ChangeDirectory(pid0, "/tmp/");
	client.Exec(pid0, "ndf_monitor");
	
	client.ChangeDirectory(pid1, "/tmp/");
	client.Exec(pid1, "ndf_monitor");

	while(client.Connect()) {
		if(client.IsAlive(pid0) != ClProLang::Successful)
			break;
		if(client.IsAlive(pid1) != ClProLang::Successful)
			break;
		CcTime::Sleep(1000.00f);
	}

	client.Terminate(pid0);
	client.Terminate(pid1);

	CcCore::Exit(0);
}
