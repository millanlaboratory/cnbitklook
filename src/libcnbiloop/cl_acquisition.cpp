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

#include "ClAcqAsServer.hpp" 
#include "ClAcqLang.hpp"
#include "ClNamesClient.hpp"
#include <libcnbiacq/CaWriter.hpp>
#include <libcnbiacq/CaDevice.hpp>
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcPipeServer.hpp>
#include <libcnbicore/CcServerMulti.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define PIPELINES 10

using namespace std;

void idle(void) {
	char a;
	printf("Press Enter to initiate acquisition");
	cin >> a;
}

void usage(void) { 
	printf("Usage: cl_acquisition [OPTION]...\n\n");
	printf("  -d       the device: gtec, biosemi, GDF/BDF file, EGD string\n");
	printf("  -f       the buffering rate in Hz (16 default)\n");
	printf("  -a       the TCP port for the acquisition server (9000 default)\n");
	printf("  -n       the basename for the pipes (/tmp/cl.pipe.ndf. default)\n");
	printf("  -i       interactive acquisition starting\n");
	printf("  -p       print labels added to NDF frame\n");
	printf("  -b       print EGD/NDF buffer informations\n");
	printf("  -w       warn when late\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optdevice("");
	std::string optfs("16");
	CcEndpoint optendpoint("127.0.0.1:9000");
	std::string optpipename("/tmp/cl.pipe.ndf.");
	bool optinteractive = false, optprintndf = false, optprintbuffers = false,
		 optwarnlate = false;

	while((opt = getopt(argc, argv, "d:f:l:n:hipwb")) != -1) {
		if(opt == 'd')
			optdevice.assign(optarg);
		else if(opt == 'f')
			optfs.assign(optarg);
		else if(opt == 'a')
			optendpoint.SetAddress(optarg);
		else if(opt == 'n')
			optpipename.assign(optarg);
		else if(opt == 'i')
			optinteractive = true;
		else if(opt == 'p')
			optprintndf = true;
		else if(opt == 'w')
			optwarnlate = true;
		else if(opt == 'b')
			optprintbuffers = true;
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}
	CcCore::OpenLogger("cl_acquisition");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	CcLogInfoS("Acquisition configured: " << 
			optfs << "Hz, " << 
			optpipename << ", " << 
			optendpoint.GetAddress() << "/TCP");
	CcLogInfoS("Total pipelines set to " << PIPELINES << ": " <<
			"/pipe[0," << PIPELINES << "] --> /ctrl[0," << PIPELINES << "]");
	
	// Variables for mainloop
	CcTimeValue ticSignals;
	size_t gsize = -1, asize = -1;
	
	// Prepare NDF frame and its semafore
	ndf_frame frame;
	CcSemaphore semframe;

	// Open, Setup and Start EGD device
	CaDevice eegdev;
	if(eegdev.Open(optdevice) == false) {
		CcLogFatal("Cannot open EGD device");
		CcCore::Exit(2);
	}
	if(eegdev.Setup((float)atof(optfs.c_str())) == false) {
		CcLogFatal("Cannot setup EGD device");
		CcCore::Exit(3);
	}
	// Initialize XDF writer and NDF frame
	CaWriter writer(&eegdev);
	eegdev.InitNDF(&frame);

	// Setup XDF writer
	std::string tmpfn;
	CcTime::Timestamp(&tmpfn);
	tmpfn.append(".bdf");

	// Setup CcPipeServer
	CcPipeServer* pipes;
	pipes = new CcPipeServer(frame.ack.buffer, frame.ack.bsize,
			frame.data.bsize);
	pipes->Open(optpipename, PIPELINES);

	// Setup, Bind and register ClAcqAsServer
	/* 2011-05-12  Michele Tavella <michele.tavella@epfl.ch>
	 * Found delay in starting iD communication. Must be inspected.
	 * This is a temporary patch/hack.
	 */
	CcServerMulti server(true, 100.00f, CCCORE_1MB);
	ClAcqAsServer handler(&writer, &frame, &semframe);
	ClNamesClient nsclient;
	try {
		handler.Register(&server);
		server.Bind(optendpoint, 2);
	} catch(CcException e) {
		CcLogFatal("Cannot bind socket");
		CcCore::Exit(1);
	}
	if(nsclient.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		CcCore::Exit(2);
	}
	int nsstatus = nsclient.Set("/acquisition", server.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		CcCore::Exit(3);
	}
	
	// Register pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream pipename, pipepath;
		pipename << "/pipe" << p;
		pipepath << optpipename << p;
		int nsstatus = nsclient.Set(pipename.str(), pipepath.str());
		if(nsstatus != ClNamesLang::Successful) {
			CcLogFatal("Cannot register pipes with nameserver");
			CcCore::Exit(5);
		}
	}
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream ctlname, ctrladdr;
		ctlname << "/ctrl" << p;
		ctrladdr << "127.0.0.1:590" << p;
		int nsstatus = nsclient.Set(ctlname.str(), ctrladdr.str());
		if(nsstatus != ClNamesLang::Successful) {
			CcLogFatal("Cannot register controllers with nameserver");
			CcCore::Exit(6);
		}
	}

	if(optinteractive == true)
		idle();

	if(eegdev.Start() == false) {
		CcLogFatal("Cannot start EGD device");
		goto shutdown;
	}
	CcLogInfo("Started EGD device");

	CcTime::Tic(&ticSignals);
	while(true) {
		gsize = eegdev.GetData();
		asize = eegdev.GetAvailable();
		if(gsize == (size_t)-1) {
			CcLogFatal("EGD device is down");
			break;
		}
		writer.Write(gsize);
		
		semframe.Wait();
		eegdev.WriteNDF(&frame);
		pipes->Write(frame.data.buffer, 0);
		if(optprintndf) 
			ndf_print_labels(&frame);
		ndf_clear_labels(&frame);
		semframe.Post();
		
		if(CcTime::Toc(&ticSignals) >= 500.00f) {
			if(nsclient.IsConnected() == false)  {
				CcLogFatal("Lost connection with nameserver");
				break;
			}
			if(CcCore::receivedSIGAny.Get()) 
				break;
			CcTime::Tic(&ticSignals);
		}
		if(asize > 0 && optwarnlate)
			CcLogWarningS("Running late: Get/Avail=" << gsize << "/" << asize);
		if(optprintbuffers) 
			printf("Get=%5.5lu, Avail=%5.5lu %s\n", 
					gsize, asize, asize > 0 ? "Warning: running late!" : "");
	}

shutdown:
	// Stop and unregister ClAcqAsServer
	server.Release();
	server.Join();
	nsclient.Unset("/acquisition");
	
	// Deregister pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream pipename;
		pipename << "/pipe" << p;
		nsclient.Unset(pipename.str());
	}
	// Deregister pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream ctrl;
		ctrl << "/ctrl" << p;
		nsclient.Unset(ctrl.str());
	}

	if(writer.Close() == false)
		CcLogError("Cannot stop XDF writer");
	if(eegdev.Stop() == false)
		CcLogError("Cannot stop EGD device");
	if(eegdev.Close() == false) 
		CcLogError("Cannot close EGD device");

	delete pipes;
	ndf_free(&frame);

	CcCore::Exit(0);
}
