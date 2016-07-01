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

#include "ClLoop.hpp"
#include "ClLoopConfigSrv.hpp"
#include "ClAcqServer.hpp" 
#include "ClDevServer.hpp" 
#include "ClBusServer.hpp" 
#include "ClAcqLang.hpp"
#include "ClNmsClient.hpp"
#include <cnbiacq/CaWriter.hpp>
#include <cnbiacq/CaDevice.hpp>
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcPipeServer.hpp>
#include <cnbicore/CcServer.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Includes for scope
extern "C" {
#include <mcpanel.h>
}
#include <string.h>

#define PIPELINES 15

using namespace std;

// This program is a simple fusion of the eegview scope and the cl_acquisition program of the cnbiloop
// so that the eegview scope can run un parallel to the acquisition, what is not possible with the conventional
// cl_acquisition (because both programs need to share the eegdev resources)


// Scope related code (variables and functions)
/**************************************************************************
 *                                                                        *
 *              Global variables                                          *
 *                                                                        * 
 **************************************************************************/
static const char* uifilename = NULL;

#define NSCALE 2
static const char* scale_labels[NSCALE] = {"25.0mV", "50.0mV"};
static const float scale_values[NSCALE] = {25.0e3, 50.0e3};

#define NTAB 3
static struct panel_tabconf tabconf[NTAB] = {
	{TABTYPE_SCOPE,"EEG"},
	{TABTYPE_BARGRAPH, "Offsets", NSCALE, scale_labels, scale_values},
	{TABTYPE_SCOPE,"Sensors"}
};

static char devinfo[1024];

struct auxdev {
	mcpanel* _panel;
	unsigned int sf;
	unsigned int neeg;
	unsigned int nexg;
	unsigned int ntrig;
	const char* devtype;
	const char* devid;
	const char* prefiltering;
} _auxdev;

static
void on_device_info(int id, void* data)
{
	fprintf(stdout,"AAA");
	(void)id;
	mcpanel* panel = (mcpanel*)data;

	snprintf(devinfo, sizeof(devinfo)-1,
	       "system info:\n\n"
	       "device type: %s\n"
	       "device model: %s\n"
	       "sampling frequency: %u Hz\n"
	       "num EEG channels: %u\n"
	       "num sensor channels: %u\n"
	       "num trigger channels: %u\n"
	       "prefiltering: %s\n",
	       _auxdev.devtype, _auxdev.devid, _auxdev.sf,
	       _auxdev.neeg, _auxdev.nexg, _auxdev.ntrig, _auxdev.prefiltering);
	
	mcp_popup_message(panel, devinfo);	
}


// Acquisition related code (functions idle, usage and main)
void idle(void) {
	char a;
	printf("Press Enter to initiate acquisition");
	cin >> a;
}

void usage(void) { 
	printf("Usage: cl_acquisition [OPTION]...\n\n");
	printf("  -d       device: gtec, biosemi, GDF/BDF file, EGD string\n");
	printf("  -f       buffering rate in Hz (16 default)\n");
	printf("  -n       basename for the pipes (/tmp/cl.pipe.ndf. default)\n");
	printf("  -i       interactive acquisition starting\n");
	printf("  -p       print labels added to NDF frame\n");
	printf("  -b       print EGD/NDF buffer informations\n");
	printf("  -w       warn when late\n");
	printf("  -r       re-open device once down\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char* argv[]) {

	int opt;
	std::string optdevice("");
	std::string optfs("16");
	std::string optpipename("/tmp/cl.pipe.ndf.");
	bool optinteractive = false, optprintndf = false, optprintbuffers = false,
		 optwarnlate = false, optreopen = false;

	while((opt = getopt(argc, argv, "d:f:l:n:hipwbr")) != -1) {
		if(opt == 'd')
			optdevice.assign(optarg);
		else if(opt == 'f')
			optfs.assign(optarg);
		else if(opt == 'n')
			optpipename.assign(optarg);
		else if(opt == 'i')
			optinteractive = true;
		else if(opt == 'r')
			optreopen = true;
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
	ClLoop::Configure("", true);
	
	CcLogConfigS("Acquisition: " << 
			optfs << "Hz, " << 
			optpipename << "[0," << PIPELINES-1 << "]");
	CcLogConfigS("Pipelines " << PIPELINES << ": " <<
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
	CcLogConfigS("NDF/EGD configured:" <<
			" Sf=" << frame.config.sf << " Hz" <<
			", Block=" << frame.config.samples << " samples" << 
			", EEG=" << frame.config.eeg_channels <<
			", EXG=" << frame.config.exg_channels <<
			", TRI=" << frame.config.tri_channels);

	// Setup XDF writer
	std::string tmpfn;
	CcTime::Timestamp(&tmpfn);
	tmpfn.append(".bdf");

	// Setup CcPipeServer
	CcPipeServer* pipes;
	pipes = new CcPipeServer(frame.ack.buffer, frame.ack.bsize,
			frame.data.bsize);
	pipes->Open(optpipename, PIPELINES);
	
	CcLogConfigS("Acq configured as: " << ClLoopConfigSrv::GetSrvAcq());
	CcLogConfigS("Bus configured as: " << ClLoopConfigSrv::GetSrvBus());
	CcLogConfigS("Dev configured as: " << ClLoopConfigSrv::GetSrvDev());

	// Initialize nameserver client
	ClNmsClient nsclient;
	int nsstatus;
	
	CcServer serverAcq(CCCORE_1MB);
	ClAcqServer handleAcq(&writer);
	handleAcq.Register(&serverAcq);
	if(serverAcq.Bind(ClLoopConfigSrv::GetSrvAcq()) == false) {
		CcLogFatal("Cannot bind acquisition socket");
		CcCore::Exit(4);
	}

	CcServer serverBus(CCCORE_1MB);
	ClBusServer handleBus(&writer, &frame, &semframe);
	handleBus.Register(&serverBus);
	if(serverBus.Bind(ClLoopConfigSrv::GetSrvBus()) == false) {
		CcLogFatal("Cannot bind bus socket");
		CcCore::Exit(5);
	}
	
	CcServer serverDev(CCCORE_1MB);
	ClDevServer handleDev(&writer, &frame, &semframe);
	handleDev.Register(&serverDev);
	if(serverDev.Bind(ClLoopConfigSrv::GetSrvDev()) == false) {
		CcLogFatal("Cannot bind dev socket");
		CcCore::Exit(50);
	}

	if(nsclient.Connect(ClLoopConfigSrv::GetNms()) == false) {
		CcLogFatal("Cannot connect to nameserver");
		CcCore::Exit(6);
	}

	nsstatus = nsclient.Set("/acquisition", ClLoopConfigSrv::GetAcq());
	if(nsstatus != ClNmsLang::Successful) {
		CcLogFatal("Cannot register acquisition with nameserver");
		CcCore::Exit(7);
	}
	
	nsstatus = nsclient.Set("/bus", ClLoopConfigSrv::GetBus());
	if(nsstatus != ClNmsLang::Successful) {
		CcLogFatal("Cannot register bus with nameserver");
		CcCore::Exit(8);
	}
	
	nsstatus = nsclient.Set("/dev", ClLoopConfigSrv::GetDev());
	if(nsstatus != ClNmsLang::Successful) {
		CcLogFatal("Cannot register dev with nameserver");
		CcCore::Exit(8);
	}
	
	// Register pipes on nameserver
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream pipename, pipepath;
		pipename << "/pipe" << p;
		pipepath << optpipename << p;
		int nsstatus = nsclient.Set(pipename.str(), pipepath.str());
		if(nsstatus != ClNmsLang::Successful) {
			CcLogFatal("Cannot register pipes with nameserver");
			CcCore::Exit(9);
		}
	}
	for(int p = 0; p < PIPELINES; p++) { 
		std::stringstream ctlname, ctrladdr;
		ctlname << "/ctrl" << p;
		ctrladdr << ClLoopConfigSrv::GetSrvIp() << ":950" << p;
		int nsstatus = nsclient.Set(ctlname.str(), ctrladdr.str());
		if(nsstatus != ClNmsLang::Successful) {
			CcLogFatal("Cannot register controllers with nameserver");
			CcCore::Exit(10);
		}
	}

	if(optinteractive == true)
		idle();


	// Prepare scope
	mcpanel* panel = NULL;

	struct panel_button custom_button = {
		"device info",
		on_device_info,
		0
	};
	struct PanelCb cb = {
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"eegview", // Let's hardcode it, it will never change...
		1,
		&custom_button
	};

	// Init scope panel
	mcp_init_lib(&argc, &argv);

	panel = mcp_create(uifilename, &cb, NTAB, tabconf);
	if (!panel) {
		fprintf(stderr,"error at the creation of the panel\n");
	}
	

	const char*** clabels = eegdev.GetLabels();

	// Setup the panel with the settings
	mcp_define_tab_input(panel, 0, frame.config.eeg_channels, frame.config.sf, clabels[0]);
	mcp_define_tab_input(panel, 1, frame.config.eeg_channels, frame.config.sf, clabels[0]);
	mcp_define_tab_input(panel, 2, frame.config.exg_channels, frame.config.sf, clabels[1]);
	mcp_define_triggers(panel, 16, frame.config.sf);

	// Run the panel
	mcp_show(panel, 1);
	mcp_run(panel, 1);

	// Fill in the auxiliary struct with all info
	_auxdev.sf = frame.config.sf; //eegdev.GetFS() would be as good
	_auxdev.neeg = frame.config.eeg_channels;
	_auxdev.nexg = frame.config.exg_channels;
	_auxdev.ntrig = frame.config.tri_channels;
	_auxdev.devtype = eegdev.GetDevType();
	_auxdev.devid = eegdev.GetDevID();
	_auxdev.prefiltering = eegdev.GetPrefiltering();
	fprintf(stdout,"sf = %d\n", _auxdev.sf);
	fprintf(stdout,"neeg = %d\n", _auxdev.neeg);
	fprintf(stdout,"nexg = %d\n", _auxdev.nexg);
	fprintf(stdout,"ntrig = %d\n", _auxdev.ntrig);
	fprintf(stdout,"devtype = %s\n", _auxdev.devtype);
	fprintf(stdout,"devid = %s\n", _auxdev.devid);
	fprintf(stdout,"prefiltering = %s\n", _auxdev.prefiltering);

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
			if(optreopen == false) {
				CcLogFatal("EGD device is down");
				break;
			} else {
				CcLogWarning("EGD device is down");
				eegdev.Close();
				std::cout<<"TRYING TO RESETUP!!!!"<<std::endl;
				if(eegdev.Open(optdevice) == false) {
					CcLogFatal("Cannot re-open EGD device");
					break;
				}
				if(eegdev.Setup((float)atof(optfs.c_str())) == false) {
					CcLogFatal("Cannot re-setup EGD device");
					break;
				}
				eegdev.Start();
				CcLogInfo("EGD device was successfully re-opened");
			}
			continue;
		}
		writer.Write(gsize);
		
		semframe.Wait();
		eegdev.WriteNDF(&frame);
		pipes->Write(frame.data.buffer, 0);

		// Show in scope
		if (gsize < 0) {
			mcp_notify(panel, DISCONNECTED);
			mcp_popup_message(panel, "Error reading!");
			break;
		}

		mcp_add_samples(panel, 0, gsize, (float*)frame.offset.eeg);
		mcp_add_samples(panel, 1, gsize, (float*)frame.offset.eeg);
		mcp_add_samples(panel, 2, gsize, (float*)frame.offset.exg);
		mcp_add_triggers(panel, gsize, (const uint32_t*)frame.offset.tri);		

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

	// Destroy scope
	mcp_destroy(panel);

	// Stop and unregister ClAcqServer
	serverAcq.Release();
	nsclient.Unset("/acquisition");
	
	serverBus.Release();
	nsclient.Unset("/bus");
	
	serverDev.Release();
	nsclient.Unset("/dev");
	
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
