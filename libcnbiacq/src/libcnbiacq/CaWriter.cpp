/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiacq library

    The libcnbiacq library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CAWRITER_CPP 
#define CAWRITER_CPP 

#include "CaWriter.hpp" 
#include <libtobicore/TCBlock.hpp>
#include <libtobicore/TCTimestamp.hpp>
#include <errno.h>
#include <string.h>


CaWriter::CaWriter(CaDevice* device) {
	this->_file = NULL;
	this->_device = device;
}

CaWriter::~CaWriter(void) {
	if(this->IsOpen())
		this->Close();
}
		
bool CaWriter::Setup(CaDevice* device) {
	this->_semlock.Wait();
	if(device == NULL && this->_device == NULL) {
		CcLogErrorS("Setup requires a CaDevice to be set");
		return false;
	} else if(device != NULL) {
		this->_device = device;
	} 

	if(xdf_set_conf(this->_file,
			XDF_F_REC_DURATION, 1.0,
			XDF_F_REC_NSAMPLE, this->_device->_cap.sampling_freq,
			XDF_NOF) == -1) {
		CcLogErrorS("Cannot setup header: " << strerror(errno));
		goto failure;
	}

	if(SetupChannelGroup(0) == -1) {
		CcLogErrorS("Cannot setup EEG group: " << strerror(errno));
		goto failure;
	}
	if(SetupChannelGroup(1) == -1) {
		CcLogErrorS("Cannot setup EXG group: " << strerror(errno));
		goto failure;
	}
	if(SetupChannelGroup(2) == -1) {
		CcLogErrorS("Cannot setup TRI group: " << strerror(errno));
		goto failure;
	}

	if(xdf_define_arrays(this->_file, 3, this->_device->_strides) == -1) {
		CcLogErrorS("Cannot define strides:" << strerror(errno));
		goto failure;
	}

	if(xdf_prepare_transfer(this->_file) == -1) {
		CcLogErrorS("Cannot prepare transfer: " << strerror(errno));
		goto failure;
	}

	this->_semlock.Post();
	return true;

failure:
	this->_semlock.Post();
	return true;
}

bool CaWriter::Open(const std::string& filename) {
	enum xdffiletype type;

	if(filename.find(".bdf") != std::string::npos) {
		type = XDF_BDF;
		CcLogConfig("XDF filetype detected: BDF");
	} else if(filename.find(".gdf") != std::string::npos) {
		type = XDF_GDF2;
		CcLogConfig("XDF filetype detected: GDFv2");
	} else {
		type = XDF_GDF2;
		CcLogWarning("Cannot detect filetype, defaulting to GDFv2");
	}
	return this->Open(filename, type);
}

bool CaWriter::Close(void) {
	this->_semlock.Wait();
	if(this->_file == false) {
		CcLogDebug("File is not open");
		goto failure;
	}

	if(xdf_close(this->_file) == -1) {
		CcLogErrorS("Cannot close file: " << strerror(errno));
		goto failure;
	}
	this->_file = NULL;
	this->_semlock.Post();
	return true;

failure:
	this->_semlock.Post();
	return true;
}

bool CaWriter::IsOpen(void) {
	bool status = false;
	this->_semlock.Wait();
	status = (this->_file != NULL);
	this->_semlock.Post();
	return status;
}

int CaWriter::Write(int nswrite) {
	this->_semlock.Wait();
	if(this->_file == NULL) {
		this->_semlock.Post();
		return -1;
	}

	int nswritten = xdf_write(this->_file, nswrite, 
			this->_device->_eeg, 
			this->_device->_exg, 
			this->_device->_tri);
	this->_semlock.Post();
	
	return nswritten;
}

int CaWriter::SetupChannelGroup(int igrp) {
	char label[32], transducter[128], unit[16];
	double mm[2];
	int isint;
	struct xdfch* ch;

	egd_channel_info(this->_device->_dev, 
			this->_device->_grp[igrp].sensortype, 
			0,
			EGD_UNIT, unit,
			EGD_TRANSDUCTER, transducter,
			EGD_MM_D, mm,
			EGD_ISINT, &isint,
			EGD_EOL);

	xdf_set_conf(this->_file, 
			XDF_CF_ARRINDEX, igrp,
			XDF_CF_ARROFFSET, 0,
			XDF_CF_ARRDIGITAL, 0,
			XDF_CF_ARRTYPE, isint ? XDFINT32 : XDFFLOAT,
			XDF_CF_PMIN, mm[0],
			XDF_CF_PMAX, mm[1],
			XDF_CF_TRANSDUCTER, transducter,
			XDF_CF_UNIT, unit,
			XDF_NOF);

	for(unsigned int j = 0; j < this->_device->_grp[igrp].nch; j++) {
		egd_channel_info(this->_device->_dev,
				this->_device->_grp[igrp].sensortype, j, 
				EGD_LABEL, label, EGD_EOL);
		if((ch = xdf_add_channel(this->_file, label)) == NULL)
			return -1;
	}
	return 0;
}

bool CaWriter::Open(const std::string& filename, enum xdffiletype type) {
	this->_semlock.Wait();
	if(this->_file != NULL) {
		CcLogError("File is already open");
		goto failure;
	}

	this->_file = xdf_open(filename.c_str(), XDF_WRITE, type);
	if(this->_file == NULL) {
		CcLogErrorS("Cannot open file: " << strerror(errno));
		goto failure;
	}
	this->_relative.Tic();
	if(type == XDF_GDF2) {
		if(this->SetupEvents() == false) {
			CcLogErrorS("Cannot setup events");
			xdf_close(this->_file);
			this->_file = NULL;
			this->_semlock.Post();
			return false;
		}
	}
	this->_semlock.Post();
	return true;

failure:
	this->_semlock.Post();
	return false;
}

double CaWriter::TocOpen(void) {
	this->_semlock.Wait();
	double ms = -1;
	if(this->_file != NULL)
		ms = this->_relative.Toc();
	this->_semlock.Post();
	return ms;
}
		
void CaWriter::Tic(TCBlock* block) {
	this->_semlock.Wait();
	block->relative.Set(&this->_relative.timestamp);
	this->_semlock.Post();
}
		
bool CaWriter::AddEvent(int event, double duration) {
	if(xdf_add_event(this->_file, event, this->TocOpen(), duration) == -1) {
		CcLogErrorS("Cannot add event " << event << ": " << strerror(errno));
		return false;
	}
	return true;
}
		
bool CaWriter::SetupEvents(void) {
	for(int i = 0; i < 65536; i++) {
		if(xdf_add_evttype(this->_file, i, "") == -1) {
			CcLogErrorS("Cannot add evttype: " << strerror(errno));
			return false;
		}
	}
	//xdf_add_evttype(this->_file, 0x0300, "trial");
	//xdf_add_evttype(this->_file, 0x8300, "trialoff");
	//xdf_add_evttype(this->_file, 0x030d, "cfeedback");
	//xdf_add_evttype(this->_file, 0x830d, "cfeedbackoff");
	//xdf_add_evttype(this->_file, 0x0381, "targethit");
	//xdf_add_evttype(this->_file, 0x0382, "targetmiss");
	//xdf_add_evttype(this->_file, 0x0311, "beep");
	//xdf_add_evttype(this->_file, 0x030f, "cueundef");
	//xdf_add_evttype(this->_file, 0xFFFF, "wait");
	//xdf_add_evttype(this->_file, 0x0312, "fixation");

	//xdf_add_evttype(this->_file, 0x0302, "Right_Hand_MI");
	//xdf_add_evttype(this->_file, 0x0301, "Left_Hand_MI");
	//xdf_add_evttype(this->_file, 0x0305, "Both_Hands_MI");
	//xdf_add_evttype(this->_file, 0x0303, "Both_Feet_MI");
	//xdf_add_evttype(this->_file, 0x030f, "Rest_MI");
	//xdf_add_evttype(this->_file, 0x0304, "Tongue_MI");
	//xdf_add_evttype(this->_file, 0x030b, "ErrP_Detected");
	//xdf_add_evttype(this->_file, 0x030c, "ErrP_NotDetected");
	//xdf_add_evttype(this->_file, 0x0010, "MI_Onset_Detected");
	//xdf_add_evttype(this->_file, 0x0011, "MI_Onset_NotDetected");
	return true;
}
#endif
