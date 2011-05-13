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
#include <libcnbicore/CcPipeSource.hpp>
#include <libcnbicore/CcSemaphore.hpp>
#include <libcnbicore/CcThread.hpp>
#include <libcnbicore/CcBasic.hpp>
#include <iostream>
#include <string.h>

class Writer : public CcThread {
	public:
		void Main(void) {
			unsigned int data = 0;
			CcPipeSource pipe("/tmp/ccpipe.1");
			
			try { 
				pipe.Open();
			} catch(CcException e) {
				this->Stop();
			}
			CcLogInfo("Sink attached, time to write");

			size_t wsize;
			bool ontime = true;
			while(CcThread::IsRunning() && pipe.IsBroken() == false) {
				data++;
				wsize = pipe.Write(&data, sizeof(unsigned int));
				
				/* Will never enter here in this example, if it does, 
				 * you are screeched :-)
				 */
				if(ontime == false) {
					std::cout << "Skipping: " << data << std::endl;
					continue;
				}
					 
				if(wsize == TP_BROKENS) {
					std::cout << "Pipe is broken" << std::endl;
					this->Stop();
				}

				if(wsize <= 0) {
					std::cout << "Sink is down" << std::endl;
					this->Stop();
				}
				std::cout << "Writing: " << data << std::endl;
				std::cout << "  " << wsize << std::endl;
				CcTime::Sleep(500);
			}

			try { pipe.Close(); } catch(CcException e) { }
			CcLogWarning("Leaving main loop");
		}
};

#define SIZE_T_MAX ((size_t) -1)

int main(void) {
	CcCore::OpenLogger("ccpipesource", 
			CcCore::TerminalColors, CcCore::LevelDebug);

	CcPipeSource::CatchSIGPIPE();
	Writer w;
	CcLogInfo("Starting writer");
	w.Start();

	while(w.IsRunning()) { 
		std::cout << tp_receivedsigpipe() << std::endl;
		CcTime::Sleep(1000);
	}

	w.Join();
	CcLogInfo("Going down");

	return 0;
}
