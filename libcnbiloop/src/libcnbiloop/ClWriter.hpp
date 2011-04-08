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

#ifndef CLWRITER_HPP 
#define CLWRITER_HPP 

#include "ClDevice.hpp"
#include <libcnbicore/CcBasic.hpp>
#include <libtobiid/IDMessage.hpp>
#include <libtobicore/TCTimestamp.hpp>
#include <xdfio.h>

class ClWriter {
	public:
		ClWriter(ClDevice* device = NULL);
		virtual ~ClWriter(void);
		virtual bool Setup(ClDevice* device = NULL);
		virtual bool Open(const std::string& filename);
		virtual bool Open(const std::string& filename, enum xdffiletype type);
		virtual bool Close(void);
		virtual bool IsOpen(void);
		virtual int Write(int nswrite);
		virtual double TocOpen(void);
		virtual void Tic(TCBlock* block);
	protected:
		virtual int SetupChannelGroup(int igrp);

	protected:
		struct xdf* _file;
		ClDevice* _device;
		std::stringstream _stream;
		CcSemaphore _semlock;
		TCTimestamp _relative;
};

#endif
