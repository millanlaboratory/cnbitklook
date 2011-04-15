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
#include "ClWriter.hpp"
#include "ClNamesClient.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcPipeServer.hpp>
#include <libcnbicore/CcServerMulti.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void usage(void) { 
	printf("Usage: cl_acquisition ");
	printf("-d [DEV] [-f HZ -a IP:PORT -n NAME]\n");
	printf("Where: -d       gTec/Biosemi ID, GDF/BDF filename\n");
	printf("       -f       16 (default), 32, 64...\n");
	printf("       -a       9000 (default)\n");
	printf("       -n       /tmp/cl.pipe.ndf. (default)\n");
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optdevice;
	std::string optfs("16");
	CcEndpoint optendpoint("127.0.0.1:9000");
	std::string optpipename("/tmp/cl.pipe.ndf.");

	while ((opt = getopt(argc, argv, "d:f:p:n:h")) != -1) {
		if(opt == 'd')
			optdevice.assign(optarg);
		else if(opt == 'f')
			optfs.assign(optarg);
		else if(opt == 'a')
			optendpoint.SetAddress(optarg);
		else if(opt == 'n')
			optpipename.assign(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}
	CcCore::OpenLogger("cl_acquisition");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();

	CcLogInfo(std::string("Acquisition configured: ").
			append(optfs).append("Hz, ").
			append(optpipename).append(", ").
			append(optendpoint.GetAddress()).append("/TCP"));
	
	// Prepare NDF frame and its semafore
	ndf_frame frame;
	CcSemaphore semframe;
	
	// Open, Setup and Start EGD device
	ClDevice eegdev;
	if(eegdev.Open(optdevice) == false) {
		CcLogFatal("Cannot open EGD device");
		exit(2);
	}
	if(eegdev.Setup((float)atof(optfs.c_str())) == false) {
		CcLogFatal("Cannot setup EGD device");
		exit(3);
	}
	if(eegdev.Start() == false) {
		CcLogFatal("Cannot start EGD device");
		exit(4);
	}
	// Initialize NDF frame according to EGD preferences
	eegdev.InitNDF(&frame);

	// Setup XDF writer
	std::string tmpfn;
	CcTime::Timestamp(&tmpfn);
	tmpfn.append(".bdf");
	ClWriter writer(&eegdev);

	// Setup CcPipeServer
	CcPipeServer* pipes;
	pipes = new CcPipeServer(frame.ack.buffer, frame.ack.bsize,
			frame.data.bsize);
	pipes->Open(optpipename, 6);

	// Setup, Bind and register ClAcqAsServer
	CcServerMulti server(true, 5*CCCORE_1MB);
	ClAcqAsServer handler(&writer, &frame, &semframe);
	ClNamesClient nsclient;
	try {
		handler.Register(&server);
		server.Bind(optendpoint, 2);
	} catch(CcException e) {
		CcLogFatal("Cannot bind socket");
		exit(1);
	}
	if(nsclient.Connect() == false) {
		CcLogFatal("Cannot connect to nameserver");
		exit(2);
	}
	int nsstatus = nsclient.Set("/acquisition", server.GetLocal());
	if(nsstatus != ClNamesLang::Successful) {
		CcLogFatal("Cannot register with nameserver");
		exit(3);
	}
	
	// Register pipes on nameserver
	for(int p = 0; p < 6; p++) { 
		std::stringstream pipename, pipepath;
		pipename << "/pipe" << p;
		pipepath << optpipename << p;
		int nsstatus = nsclient.Set(pipename.str(), pipepath.str());
		if(nsstatus != ClNamesLang::Successful) {
			CcLogFatal("Cannot register pipes with nameserver");
			exit(5);
		}
	}

	CcLogInfo("Starting acquisiton");
	CcTimeValue tvSigCheck;
	CcTime::Tic(&tvSigCheck);
	size_t gsize = -1, asize = -1;
	while(true) {
		gsize = eegdev.GetData();
		asize = eegdev.GetAvailable();
		if(gsize == (size_t)-1) {
			CcLogFatal("Device is down");
			break;
		}
		writer.Write(gsize);
		
		semframe.Wait();
		eegdev.WriteNDF(&frame);
		pipes->Write(frame.data.buffer, 0);
		// TODO: write to file
		//ndf_print_labels(&frame);
		ndf_clear_labels(&frame);
		semframe.Post();
		
		if(CcTime::Toc(&tvSigCheck) >= 2000.00f) {
			if(nsclient.Connect() == false)  {
				CcLogFatal("Lost connection with nameserver");
				break;
			}
			if(CcCore::receivedSIGAny.Get()) 
				break;
			CcTime::Tic(&tvSigCheck);
		}
	}

	// Stop and unregister ClAcqAsServer
	server.Release();
	server.Join();
	nsclient.Unset("/acquisition");
	
	// Register pipes on nameserver
	for(int p = 0; p < 6; p++) { 
		std::stringstream pipename;
		pipename << "/pipe" << p;
		nsclient.Unset(pipename.str());
	}

	CcLogInfo("Stopping acquisition");

	if(writer.Close() == false)
		CcLogError("Cannot stop XDF writer");
	if(eegdev.Stop() == false)
		CcLogError("Cannot stop EGD device");
	if(eegdev.Close() == false) 
		CcLogError("Cannot close EGD device");

	delete pipes;
	ndf_free(&frame);

	return 0;
}
