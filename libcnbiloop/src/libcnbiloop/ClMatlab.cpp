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

#ifndef CLMATLAB_CPP 
#define CLMATLAB_CPP 

#include <libcnbicore/CcBasic.hpp>
#include "ClMatlab.hpp" 

ClMatlab::ClMatlab(const std::string& cmd, bool write, bool read) : 
	CcProcess(cmd, write, read) {
}

ClMatlab::~ClMatlab(void) {
}

void ClMatlab::Exec(void) { 
	execlp(this->_cmd.c_str(), 
		"-nodesktop", "-nojvm", "-nosplash",
		"2>1", NULL);
}

void ClMatlab::AddPath(const std::string& path) {
	char buffer[2048];
	char format[2048] = "try; status = exist('%s', 'dir'); \
						 if(status == false); exit; else; addpath('%s'); end; \
						 catch e; exit; end\n";
	sprintf(buffer, format, path.c_str(), path.c_str());

	CcLogDebug(std::string("Adding path: ").append(buffer));
	CcProcess::Write(buffer);
}

void ClMatlab::Include(const std::string& path0, const std::string& path1) {
	char buffer[2048];
	char format[2048] = "try; status0 = mtpath_include('%s'); \
						 if(status0 == false); status1 = mtpath_include('%s'); \
						 end; if(status0 == false && status1 == false); exit; \
						 end; catch e; exit; end\n";
	sprintf(buffer, format, path0.c_str(), path1.c_str());
	
	CcLogDebug(std::string("Including path: ").append(buffer));
	CcProcess::Write(buffer);
}
		
void ClMatlab::ChangeDirectory(const std::string& path) {
	char buffer[2048];
	char format[2048] = "try; status = exist('%s', 'dir'); cd('%s'); \
						 if(status == false); exit; end; catch e; exit; end\n";
	sprintf(buffer, format, path.c_str(), path.c_str());

	CcLogDebug(std::string("Changing directory: ").append(buffer));
	CcProcess::Write(buffer);
}

void ClMatlab::Launch(const std::string function) {
	char buffer[2048];
	char format[2048] = "try; %s; catch e; exit; end\n";
	
	sprintf(buffer, format, function.c_str());
	CcLogDebug(std::string("Lanching: ").append(buffer));
	CcProcess::Write(buffer);
}

void ClMatlab::LaunchNDF(const std::string function, const std::string& pipename, 
		const CcAddress addressD, const CcAddress addressC, 
		const std::string& extra) {
	char buffer[2048];
	char format[2048] = "try; while(1);%s('%s', '%s', '%s'%s);end; \
						 catch e; exit; end\n";

	std::string optional("");
	if(extra.empty() == false) {
		optional.assign(", ");
		optional.append(extra);
	}
	
	sprintf(buffer, format, function.c_str(), pipename.c_str(),
			addressD.c_str(), addressC.c_str(), optional.c_str());

	CcLogDebug(std::string("Lanching NDF: ").append(buffer));
	CcProcess::Write(buffer);
}

#endif
