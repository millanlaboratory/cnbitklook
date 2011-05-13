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

#include <libcnbicore/CcPipeReader.hpp>
#include <libcnbicore/CcPipeWriter.hpp>
#include <libcnbicore/CcBasic.hpp>
#include <iostream>
#include <string.h>

int main(void) {
	CcCore::OpenLogger("ccpipereader", 
			CcCore::TerminalColors, CcCore::LevelConfig);

	CcPipeWriter w;
	CcPipeReader r;

	w.Open("/tmp/ccpipeloop.0");
	r.Open("/tmp/ccpipeloop.0");
	
	std::string message;
	bool hasmessage = false;
	CcTime::Sleep(2000);

	CcTimeValue tic;
	CcLogInfo("Entering loop");
	while(true) { 
		CcTime::Tic(&tic);
		w.Write("<hello>world</hello>");
		hasmessage = r.datastream.Extract(&message, 
				std::string("<hello>"), std::string("</hello>"), 
				CcStreamer::Forward);
		if(hasmessage) {
			std::cout << "Received \"" << message << 
				"\" in ~" << CcTime::Toc(&tic) << "ms" << 
				" with " << r.datastream.Count(std::string("<hello>")) << 
				" leftovers" << std::endl;
		}
		CcTime::Sleep(500.00f);
	}

	r.Join();
	w.Join();

	return 0;
}
