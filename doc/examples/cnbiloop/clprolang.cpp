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

#include <libcnbiloop/ClProLang.hpp>
#include <iostream>

using namespace std;

int main(void) {
	int pid;
	int id;
	std::string call, path0, path1;
	std::string pipename, function, addressD, addressC, extra;

	ClProLang pro;
	cout << pro.Fork() << endl;
	cout << pro.IsFork(pro.message->buffer) << endl;
	
	cout << pro.Launch(100, "bench(1);") << endl;
	cout << pro.IsLaunch(pro.message->buffer, &pid, &call) << endl;

	cout << pro.Terminate(100) << endl;
	cout << pro.IsTerminate(pro.message->buffer, &pid) << endl;
	
	cout << pro.Error(pro.NotUndestood) << endl;
	cout << pro.IsError(pro.message->buffer, &id) << endl;
	
	cout << pro.Ok(100) << endl;
	cout << pro.IsOk(pro.message->buffer, &pid) << endl;
	
	cout << pro.ChangeDirectory(100, "/home/mtavella") << endl;
	cout << pro.IsChangeDirectory(pro.message->buffer, &pid, &path0) << endl;
	
	cout << pro.Include(100, "/home/mtavella", "/tmp/mtavella") << endl;
	cout << pro.IsInclude(pro.message->buffer, &pid, &path0, &path1) << endl;
	
	function.clear();
	pipename.clear();
	addressD.clear();
	addressC.clear();
	extra.clear();
	cout << pro.LaunchNDF(100,
			"ndf_monitor", 
			"/tmp/pipe.whatever", 
			"127.0.0.1:9000", 
			"127.0.0.1:9500") << endl;
	cout << pro.IsLaunchNDF(pro.message->buffer, 
			&pid,
			&function,
			&pipename,
			&addressD,
			&addressC) << endl;
	cout << function << endl;
	cout << pipename << endl;
	cout << addressD << endl;
	cout << addressC << endl;
	
	function.clear();
	pipename.clear();
	addressD.clear();
	addressC.clear();
	extra.clear();
	cout << pro.LaunchNDF(100,
			"ndf_monitor", 
			"/tmp/pipe.whatever", 
			"127.0.0.1:9000", 
			"127.0.0.1:9500") << endl;
	cout << pro.IsLaunchNDF(pro.message->buffer, 
			&pid,
			&function,
			&pipename,
			&addressD,
			&addressC) << endl;
	cout << function << endl;
	cout << pipename << endl;
	cout << addressD << endl;
	cout << addressC << endl;

	return 0;
}
