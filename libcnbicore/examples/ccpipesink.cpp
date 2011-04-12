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

#include "CcPipeSink.hpp"
#include "CcSemaphore.hpp"
#include "CcThread.hpp"
#include "CcBasic.hpp"
#include <iostream>

class Reader : public CcThread {
	public:
		void Main(void) {
			unsigned int data = 0;
			CcPipeSink pipe("/tmp/ccpipe.1");
			try {
				pipe.Open(-1);
			} catch(CcException e) {
				this->Stop();
			}
			size_t rsize;
			bool ontime;
			while(CcThread::IsRunning()) {
				ontime = pipe.Read(&data, sizeof(unsigned int), &rsize);
				/* Will never enter here in this example, if it does, 
				 * you are screeched :-)
				 */
				if(ontime == false) {
					std::cout << "Skipping... " << std::endl;
					continue;
				}

				if(rsize <= 0)
					this->Stop();
				std::cout << "Reading: " << data << std::endl;

				if(data >= 20)
					this->Stop();
			}

			try { pipe.Close(); } catch(CcException e) { }
			CcLogWarning("Leaving main loop");
		}
};

int main(void) {
	CcCore::OpenLogger("ccpipesink", 
			CcCore::TerminalColors, CcCore::LevelDebug);

	Reader r;
	CcLogInfo("Starting reader");
	r.Start();
	while(r.IsRunning())
		CcTime::Sleep(1000);

	r.Join();
	CcLogInfo("Going down");

	return 0;
}
