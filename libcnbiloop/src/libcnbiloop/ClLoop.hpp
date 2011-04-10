/*
    Copyright (C) 2011 Michele Tavella <tavella.michele@gmail.com>

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

#ifndef CLLOOP_HPP 
#define CLLOOP_HPP 

#include "ClProClient.hpp"
#include "ClAcqClient.hpp"
#include "ClNamesClient.hpp"

class ClLoop {
	public:
		ClLoop(void);
		virtual ~ClLoop(void);
		virtual bool Connect(CcAddress nameserver = "127.0.0.1:8000");
		virtual void Disconnect(void);
	protected:
		virtual bool ConnectNameserver(void);
		virtual bool ConnectProcessing(void);
		virtual bool ConnectAcquisition(void);
		virtual bool QueryAddresses(void);

	public:
		ClProClient processing;
		ClAcqClient acquisition;
		ClNamesClient nameserver;
	protected:
		CcAddress _processing;
		CcAddress _acquisition;
		CcAddress _nameserver;
		std::stringstream _stream;
};

#endif
