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

#include <cnbicore/CcEndpoint.hpp>

using namespace std;

int main(void) {
	CcEndpoint e0("10.5.6.7:8000");
	cout <<  e0.GetIp()  << endl;
	cout <<  e0.GetPort()  << endl;
	cout <<  e0.GetPortUInt()  << endl << endl;
	
	e0.Clear();
	e0.SetAddress("9000");
	cout <<  e0.GetIp()  << endl;
	cout <<  e0.GetPort()  << endl;
	cout <<  e0.GetPortUInt()  << endl << endl;

	CcEndpoint e1("127.0.0.1:8000");
	cout <<  e1.GetIp()  << endl;
	cout <<  e1.GetPort()  << endl;
	cout <<  e1.GetPortUInt()  << endl << endl;
	
	CcEndpoint e2("127.0.0.1", "8000");
	cout <<  e2.GetIp()  << endl;
	cout <<  e2.GetPort()  << endl;
	cout <<  e2.GetPortUInt()  << endl << endl;

	CcEndpoint e3("127.0.0.1", 8000);
	cout <<  e3.GetIp()  << endl;
	cout <<  e3.GetPort()  << endl;
	cout <<  e3.GetPortUInt()  << endl << endl;
}
