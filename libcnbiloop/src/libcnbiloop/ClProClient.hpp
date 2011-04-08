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

#ifndef CLPROCLIENT_HPP 
#define CLPROCLIENT_HPP 

#include "ClClient.hpp"
#include "ClProLang.hpp"

class ClProClient : public ClClient {
	public:
		virtual bool Connect(const CcAddress address = "127.0.0.1:9100");
		virtual int Fork(int* rpid);
		virtual int ForkAndCheck(int* rpid, double waitms = 5000.00f);
		virtual int Launch(const int pid, const std::string& call);
		virtual int IsAlive(const int pid);
		virtual int Terminate(const int pid);
		virtual int ChangeDirectory(const int pid, const std::string& path);
		virtual int Include(const int pid, const std::string& path0, 
				const std::string& path1);
		virtual int LaunchNDF(const int pid, const std::string& function, 
				const std::string& pipename, const CcAddress addressD, 
				const CcAddress addressC, const std::string& extra = " ");
		
		virtual int IncludeNDF(const int pid);
		virtual bool Check(const int pid, double waitms = 2500.00f);
	private:
	protected:

	public:
	private:
		ClProLang _language;
	protected:
};

#endif
