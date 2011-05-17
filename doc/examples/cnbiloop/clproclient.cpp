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

	while(client.Connect()) {
		int pid0 = 0, pid1 = 0;
		
		if(client.ForkAndCheck(&pid0) != ClProLang::Successful)
			return 1;
		if(client.ForkAndCheck(&pid1) != ClProLang::Successful)
			return 1;
		cout << "Spawned " << pid0 << endl;
		cout << "Spawned " << pid1 << endl;
		
		client.ChangeDirectory(pid0, "/home/mtavella/");
		client.Exec(pid0, "a=rand(10,10);");
		client.Exec(pid0, "save;");
		client.Exec(pid0, "bench(1);");
		client.Exec(pid1, "antani;");
	
		for(int i = 0; i < 4; i++) {	
			if(client.Connect() == false)
				return 2;

			int status0 = client.IsAlive(pid0);
			int status1 = client.IsAlive(pid1);

			cout << "PID0 " << status0 << endl;
			cout << "PID1 " << status1 << endl;
	
			if(status0 != ClProLang::Successful)
				client.Terminate(pid0);
			if(status1 != ClProLang::Successful)
				client.Terminate(pid1);

			CcTime::Sleep(1000.00f);
		}
		
		client.Terminate(pid0);
		client.Terminate(pid1);

		CcTime::Sleep(5000.00f);
	}
	CcCore::Exit(0);
}
