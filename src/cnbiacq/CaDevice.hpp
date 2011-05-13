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

#ifndef CADEVICE_HPP 
#define CADEVICE_HPP 

#include <cnbicore/CcBasic.hpp>
#include <ndf/ndf_codec.h>
#include <eegdev.h>
#include <string>

typedef struct CaDeviceCap_struct {
	char* model;
	char* id;
    unsigned int sampling_freq;
    unsigned int eeg_nmax;
    unsigned int sensor_nmax;
    unsigned int trigger_nmax;
} CaDeviceCap;

class CaDevice {
	friend class CaWriter;

	public:
		CaDevice(int stype = EGD_FLOAT, int ttype = EGD_INT32);
		virtual ~CaDevice(void);
		virtual bool Setup(float hz = 16.00f);
		bool Open(const std::string& devname);
		bool Close(void);
		bool Start(void);
		bool Stop(void);
		size_t GetData(void);
		size_t GetAvailable(void);
		virtual void InitNDF(ndf_frame* frame);
		virtual void WriteNDF(ndf_frame* frame, bool inc = true, bool tic = true);
		void Dump(void);
	protected:
		void InitFrameSize(float hz);
		void InitCapabilities(void);
		void InitGroups(void);
		void InitBuffers(void);
		size_t SizeEGD(int egdtype);
		size_t SizeNDF(int egdtype);
	protected:
		struct eegdev* _dev;
		CaDeviceCap _cap;
		struct grpconf _grp[3];
		size_t _strides[3];
		void* _eeg;
		void* _exg;
		void* _tri;
		int _stype;
		int _ttype;
		size_t _frames;
		unsigned int _presetid;
};

#endif
