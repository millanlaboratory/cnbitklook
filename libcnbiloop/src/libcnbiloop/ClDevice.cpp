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

#ifndef CLDEVICE_CPP 
#define CLDEVICE_CPP 

#include "ClDevice.hpp" 
#include <libcnbicore/CcBasic.hpp>
#include <string.h>
#include <stdio.h>
#include <errno.h>

ClDevice::ClDevice(int stype, int ttype) {
	memset(this->_grp, 0, 3*sizeof(struct grpconf));
	memset(this->_strides, 0, 3*sizeof(size_t));
	this->_eeg = NULL;
	this->_exg = NULL;
	this->_tri = NULL;
	this->_stype = stype;
	this->_ttype = ttype;
	this->_frames = 0;
}

ClDevice::~ClDevice(void) {
	if(this->_eeg != NULL)
		free(this->_eeg);
	if(this->_exg != NULL)
		free(this->_exg);
	if(this->_tri != NULL)
		free(this->_tri);
}
		
void ClDevice::InitCapabilities(void) {
	egd_get_cap(this->_dev, EGD_CAP_FS, &this->_cap.sampling_freq);
	egd_get_cap(this->_dev, EGD_CAP_DEVTYPE, &this->_cap.model);
	egd_get_cap(this->_dev, EGD_CAP_DEVID, &this->_cap.id);
	this->_cap.eeg_nmax = egd_get_numch(this->_dev, EGD_EEG);
	this->_cap.trigger_nmax = egd_get_numch(this->_dev, EGD_TRIGGER);
	this->_cap.sensor_nmax = egd_get_numch(this->_dev, EGD_SENSOR);
}

void ClDevice::InitBuffers(void) {
	// Setup the strides so that we get packed data into the buffers
	this->_strides[0] = this->_grp[0].nch * this->SizeEGD(this->_stype);
	this->_strides[1] = this->_grp[1].nch * this->SizeEGD(this->_stype);
	this->_strides[2] = this->SizeEGD(this->_ttype);

	// Compute sizes so not to call malloc if size == 0
	size_t seeg = this->_strides[0]*this->_frames;
	size_t sexg = this->_strides[1]*this->_frames;
	size_t stri = this->_strides[2]*this->_frames;

	this->_eeg = seeg ? (void*)malloc(seeg) : NULL;
	this->_exg = sexg ? (void*)malloc(sexg) : NULL;
	this->_tri = stri ? (void*)malloc(stri) : NULL;
}
		
void ClDevice::InitGroups(void) {
	this->_grp[0].sensortype = EGD_EEG;
	this->_grp[0].index = 0;
	this->_grp[0].iarray = 0;
	this->_grp[0].datatype = this->_stype;
	this->_grp[0].arr_offset = 0;
	this->_grp[0].nch = this->_cap.eeg_nmax;
	
	this->_grp[1].sensortype = EGD_SENSOR;
	this->_grp[1].index = 0; 
	this->_grp[1].iarray = 1; 
	this->_grp[1].datatype = this->_stype;
	this->_grp[1].arr_offset = 0;
	this->_grp[1].nch = this->_cap.sensor_nmax;
	
	this->_grp[2].sensortype = EGD_TRIGGER;
	this->_grp[2].index = 0; 
	this->_grp[2].iarray = 2;
	this->_grp[2].datatype = this->_ttype;
	this->_grp[2].arr_offset = 0;
	this->_grp[2].nch = 1;
}

void ClDevice::InitFrameSize(float hz) {
	this->_frames = (size_t)((float)this->_cap.sampling_freq/hz);
}

bool ClDevice::Setup(float hz) {
	this->InitCapabilities();
	this->InitFrameSize(hz);
	this->InitGroups();
	this->InitBuffers();
	if(egd_acq_setup(this->_dev, 3, this->_strides, 3, this->_grp) == -1) {
		CcLogErrorS(this->_stream, "Cannot setup acquisition: " << 
				strerror(errno));
	}
	return true;
}

bool ClDevice::Open(const std::string& devname) {
	std::string devnamearg("datafile|path|");
	devnamearg.append(devname);

	this->_dev = egd_open(devnamearg.c_str());
	if(this->_dev == NULL) {
		CcLogErrorS(this->_stream, "Cannot open device: " << strerror(errno));
		return false;
	}
	return true;
}

bool ClDevice::Close(void) {
	if(egd_close(this->_dev) == -1) {
		CcLogErrorS(this->_stream, "Cannot close device: " << strerror(errno));
		return false;
	}
	return true;
}
		
bool ClDevice::Start(void) {
	if(egd_start(this->_dev) == -1) { 
		CcLogErrorS(this->_stream, "Cannot start device: " << strerror(errno));
		return false;
	}
	return true;
}

bool ClDevice::Stop(void) {
	if(egd_stop(this->_dev) == -1) { 
		CcLogErrorS(this->_stream, "Cannot stop device: " << strerror(errno));
		return false;
	}
	return true;
}

size_t ClDevice::SizeEGD(int egdtype) {
	size_t size = 0;
	switch(egdtype) {
		case EGD_INT32:
			size = sizeof(int32_t);
			break;
		case EGD_FLOAT:
			size = sizeof(float);
			break;
		case EGD_DOUBLE:
			size = sizeof(double);
			break;
		default:
			CcLogWarning("EGD type not known: forcing EGD_FLOAT");
			size = sizeof(float);
	}
	return size;
}

size_t ClDevice::SizeNDF(int egdtype) {
	size_t size = 0;
	switch(egdtype) {
		case EGD_INT32:
			size = NDF_INT32;
			break;
		case EGD_FLOAT:
			size = NDF_FLOAT;
			break;
		case EGD_DOUBLE:
			size = NDF_DOUBLE;
			break;
		default:
			CcLogWarning("NDF type not known: forcing NDF_FLOAT");
			size = NDF_FLOAT;
	}
	return size;
}
		
void ClDevice::Dump(void) {
	printf("[ClDevice::Dump] Device info:\n");
	printf(" + Capabilities:\n");
	printf(" |- Device:       %s\n", this->_cap.model);
	printf(" |- Id:           %s\n", this->_cap.id);
	printf(" |- Sf:           %u Hz\n", this->_cap.sampling_freq);
	printf(" |- Channels:     %u\n", this->_cap.eeg_nmax);
	printf(" |- Signals:      %u\n", this->_cap.sensor_nmax);
	printf(" `- Triggers:     %u\n", this->_cap.trigger_nmax);
}

size_t ClDevice::GetData(void) {
	return egd_get_data(this->_dev, this->_frames, 
			this->_eeg, this->_exg, this->_tri);
}
		
size_t ClDevice::GetAvailable(void) {
	return egd_get_available(this->_dev);
}

void ClDevice::WriteNDF(ndf_frame* frame, bool inc, bool tic) {
	if(inc)
		ndf_inc_fidx(frame);
	if(tic)
		ndf_doubletimetic(frame);

	if(this->_eeg != NULL)
		ndf_write_eeg_frame(frame, this->_eeg);
	if(this->_exg != NULL)
		ndf_write_exg_frame(frame, this->_exg);
	if(this->_tri != NULL)
		ndf_write_tri_frame(frame, this->_tri);
}

void ClDevice::InitNDF(ndf_frame* frame) {
	frame->types.tim 			= NDF_DOUBLETIME;
	frame->types.lbl 			= NDF_UINT16;
	frame->types.eeg 			= this->SizeNDF(this->_stype);
	frame->types.exg 			= this->SizeNDF(this->_stype);
	frame->types.tri 			= this->SizeNDF(this->_ttype);
	frame->config.labels 		= 32;
	frame->config.eeg_channels 	= this->_cap.eeg_nmax;
	frame->config.exg_channels 	= this->_cap.sensor_nmax;
	frame->config.tri_channels 	= this->_cap.trigger_nmax;
	frame->config.samples 		= ClDevice::_frames;;
	frame->config.sf 			= this->_cap.sampling_freq;
	frame->config.id 			= 0;
	ndf_init(frame);
}

#endif
