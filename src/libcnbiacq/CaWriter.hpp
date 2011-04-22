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

#ifndef CAWRITER_HPP 
#define CAWRITER_HPP 

#include "CaDevice.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libtobiid/IDMessage.hpp>
#include <libtobicore/TCTimestamp.hpp>
#include <xdfio.h>

class CaWriter {
	public:
		CaWriter(CaDevice* device = NULL);
		virtual ~CaWriter(void);
		virtual bool Setup(CaDevice* device = NULL);
		bool Open(const std::string& filename);
		bool Open(const std::string& filename, enum xdffiletype type);
		bool Close(void);
		bool IsOpen(void);
		int Write(int nswrite);
		virtual double TocOpen(void);
		virtual void Tic(TCBlock* block);
		bool AddEvent(int event, double duration = 0.00f);
	protected:
		int SetupChannelGroup(int igrp);

	protected:
		struct xdf* _file;
		CaDevice* _device;
		CcSemaphore _semlock;
		TCTimestamp _relative;
};

#endif
