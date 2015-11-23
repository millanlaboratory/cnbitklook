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

#include <iostream>
#include <cnbicore/CcFile.hpp>
using namespace std;

int main(void) {
	std::string buffer;

	CcFile source, dest;
	source.Load("/etc/fstab");
	source.Get(&buffer);

	dest.Set(source.Get());
	dest.Save("myfstab.txt");

	CcFile huge;
	huge.Load("/home/mtavella/Desktop/file.mat");
	cout << huge.Size() << "/" << huge.MaxSize() << endl;
	huge.Save("/home/mtavella/Desktop/file2.mat");
	
	return 0;
}
