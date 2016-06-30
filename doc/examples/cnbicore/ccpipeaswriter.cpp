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

#include <cnbicore/CcPipe.hpp>
#include <cnbicore/CcCore.hpp>
#include <stdio.h>

int main(void) {
	CcCore::OpenLogger("ccpipeasreader", CcCore::TerminalColors, CcCore::LevelDebug);
	
	CcPipe::CatchSIGPIPE();

	CcPipe writer;
	writer.Open("my.pipe", CcPipe::Writer);
	ssize_t size;
	for(int i = 0; i < 100000; i++) {
		size = writer.Write(&i, sizeof(int));
		if(size < 0)
			break;
		printf("Written %lu bytes: %d\n", size, i);
	}
	writer.Close();

	CcCore::Exit(0);
}
