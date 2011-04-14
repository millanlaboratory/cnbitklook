/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libcnbicore/CcPipeWriter.hpp>
#include <libcnbicore/CcBasic.hpp>
#include <iostream>
#include <string.h>


int main(void) {
	CcCore::OpenLogger("ccpipesource", 
			CcCore::TerminalColors, CcCore::LevelDebug);

	CcPipeWriter w;
	int ack = 6;
	w.Acknoledge(&ack, sizeof(int));
	w.Open("/tmp/ccpipe.1");
	CcLogInfo("Starting writer");

	int a = 1;
	while(w.IsRunning()) {
		if(w.IsOpen())
			w.Write(&a, sizeof(int));
		std::cout << w.IsBroken() << " " << w.IsOpen() << std::endl;
		CcTime::Sleep(1000);
	}

	w.Join();
	CcLogInfo("Going down");
	std::cout << w.IsBroken() << " " << w.IsOpen() << std::endl;

	return 0;
}
