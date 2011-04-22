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

#include "ClLoop.hpp"
#include <libndf/ndf_codec.h>
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcPipeSink.hpp>
#include <iostream>

using namespace std;

void usage(void) { 
	printf("Usage: cl_monitor [OPTION]...\n\n");
	printf("  -p       pipe name\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname, pipename;
	
	while((opt = getopt(argc, argv, "p:h")) != -1) {
		if(opt == 'p')
			optname.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}
	
	if(optname.empty()) {
		usage();
		CcCore::Exit(1);
	}
	
	CcCore::OpenLogger("cl_monitor");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	// Connect to loop
	CcLogInfo("Connecting to loop...");
	while(ClLoop::Connect() == false) {
		if(CcCore::receivedSIGAny.Get())
			CcCore::Exit(1);
		CcTime::Sleep(2000);
	}
	
	if(ClLoop::nameserver.Query(optname, &pipename) != ClNamesLang::Successful) {
		CcLogFatal("Cannot query pipe");
		CcCore::Exit(2);
	}

	CcTimeValue tvOpen, tvRead, tvLoop;
	double msOpen, msRead, msNDF, msLoop, msIdeal;
	unsigned long frametot;
	double loop = true;
	while(loop == true) {
		frametot = 0;
		ndf_frame frame;
		ndf_ack ack;

		// Setup pipe reader 
		CcPipeSink reader(pipename);
		try {
			reader.Open(-1);
			CcTime::Tic(&tvOpen);
		} catch(CcException e) {
			CcLogFatal("Cannot open pipe");
			CcTime::Sleep(2500.00);
			continue;
		}

		bool connected = false;
		size_t rsize = 0;
		connected = reader.Read(&ack.buffer, NDF_ACK_SIZET, &(ack.bsize));
		if(connected == false) {
			CcLogFatal("Source is down");
			CcCore::Exit(1);
		}
		if(ack.bsize != NDF_ACK_SIZET) {
			CcLogFatal("ACK not received correctly");
			CcCore::Exit(2);
		}

		CcLogInfo("ACK received, initializing NDF frame");
		ndf_initack(&frame, &ack);
		msIdeal = 1000.00f/((double)frame.config.sf/frame.config.samples);

		CcLogInfo("Receiving...");
		CcTime::Tic(&tvLoop);
		while(loop) { 
			fflush(stdout);
			CcTime::Tic(&tvRead);
			connected = reader.Read(frame.data.buffer, frame.data.bsize, &rsize);
			msLoop = CcTime::Toc(&tvLoop);
			msOpen = CcTime::Toc(&tvOpen);
			msRead = CcTime::Toc(&tvRead);
			msNDF  = ndf_doubletimetoc(&frame);

			if(frame.data.bsize != rsize) {
				printf("\n");
				CcLogWarning("Broken pipe");
				break;
			}
			
			if(frametot++ > 2) {
				printf("Uptime=%.2fmin, Frame=%8.8lu, ", 
						msOpen/1000/60, ndf_get_fidx(&frame));
				printf("iDt=%5.2fms, Dt=%5.2fms, Dr=%5.2fms, Dl=%5.2fms, ", 
						msIdeal, msNDF, msRead, msLoop);
				printf("Dt-iDt=%5.2fms            \r", 
						msNDF - msIdeal);
			}
			CcTime::Tic(&tvLoop);

			if(CcCore::receivedSIGAny.Get())
				loop = false;
		}
		reader.Close();
		ndf_free(&frame);
	}
	CcCore::Exit(0);
}
