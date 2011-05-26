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

#include "ClAcqAsServer.hpp" 
#include "ClTobiIdAsServer.hpp" 
#include "ClAcqLang.hpp"
#include "ClNamesClient.hpp"
#include <cnbiacq/CaWriter.hpp>
#include <cnbiacq/CaDevice.hpp>
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcPipeServer.hpp>
#include <cnbicore/CcServer.hpp>
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
	printf("  -A       the TCP port for the acquisition server (8125 default)\n");
	printf("  -B       the TCP port for the bus server (8126 default)\n");
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
	CcEndpoint optepAcq("0.0.0.0:8125"), optepBus("0.0.0.0:8126");
	std::string optpipename("/tmp/cl.pipe.ndf.");
	bool optinteractive = false, optprintndf = false, optprintbuffers = false,
		 optwarnlate = false;

	while((opt = getopt(argc, argv, "A:B:d:f:l:n:hipwb")) != -1) {
		if(opt == 'd')
			optdevice.assign(optarg);
		else if(opt == 'f')
			optfs.assign(optarg);
		else if(opt == 'A')
			optepAcq.SetPort(optarg);
		else if(opt == 'B')
			optepBus.SetPort(optarg);
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
			optpipename << "[0," << PIPELINES-1 << "]");
	CcLogInfoS("Total pipelines set to " << PIPELINES << ": " <<
			"/pipe[0," << PIPELINES-1 << "] and /ctrl[0," << PIPELINES-1 << "]");

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
			frame.data.bsize, frame.data.bsize);
	pipes->Open(optpipename, PIPELINES);

	// Initialize nameserver client
	ClNamesClient nsclient;
	int nsstatus;
	
	CcServer serverAcq(CCCORE_1MB);
	ClAcqAsServer handleAcq(&writer);
	handleAcq.Register(&serverAcq);
	if(serverAcq.Bind(optepAcq.GetPort()) == false) {
		CcLogFatal("Cannot bind acquisition socket");
		CcCore::Exit(4);
	}


	CcServer serverBus(CCCORE_1MB);
	ClTobiIdAsServer handleBus(&writer, &frame, &semframe);
	handleBus.Register(&serverBus);
	if(serverBus.Bind(optepBus.GetPort()) == false) {
		CcLogFatal("Cannot bind bus socket");
		CcCore::Exit(5);
	}

	if(nsclient.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		CcCore::Exit(6);
	}


	nsstatus = nsclient.Set("/acquisition", serverAcq.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register acquisition with nameserver");
		CcCore::Exit(7);
	}
	
	nsstatus = nsclient.Set("/bus", serverBus.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register bus with nameserver");
		CcCore::Exit(8);
	}
	
	
	// Register pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream pipename, pipepath;
		pipename << "/pipe" << p;
		pipepath << optpipename << p;
		int nsstatus = nsclient.Set(pipename.str(), pipepath.str());
		if(nsstatus != ClNamesLang::Successful) {
			CcLogFatal("Cannot register pipes with nameserver");
			CcCore::Exit(9);
		}
	}
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream ctlname, ctrladdr;
		ctlname << "/ctrl" << p;
		ctrladdr << "127.0.0.1:950" << p;
		int nsstatus = nsclient.Set(ctlname.str(), ctrladdr.str());
		if(nsstatus != ClNamesLang::Successful) {
			CcLogFatal("Cannot register controllers with nameserver");
			CcCore::Exit(10);
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
	serverAcq.Release();
	nsclient.Unset("/acquisition");
	
	serverBus.Release();
	nsclient.Unset("/bus");
	
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
