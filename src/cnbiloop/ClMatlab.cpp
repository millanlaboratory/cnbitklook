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

#ifndef CLMATLAB_CPP 
#define CLMATLAB_CPP 

#include <cnbicore/CcBasic.hpp>
#include "ClMatlab.hpp" 
#include "ClLoopConfig.hpp" 

#define CLMATLAB_INCLUDE1 "\
try;\
	if(isempty(getenv('CNBITKMAT_ROOT')));\
		disp('CNBITKMAT_ROOT unset, mtpath must be in /usr/share/cnbiloop/cnbitkmat/mtpath');\
		addpath('/usr/share/cnbiloop/cnbitkmat/mtpath');\
	end;\
	status0 = mtpath_include('%s');\
	if(status0 == false);\
		disp('Error: include failed');\
		exit;\
	end;\
catch e;\
	disp(['Exception: ' exception.message ]);\
	disp(exception);\
	disp(exception.stack);\
	disp('Killing Matlab...');\
	exit;\
end;\n"

#define CLMATLAB_INCLUDE2 "\
try;\
	if(isempty(getenv('CNBITKMAT_ROOT')));\
		disp('CNBITKMAT_ROOT unset, mtpath must be in /usr/share/cnbiloop/cnbitkmat/mtpath');\
		addpath('/usr/share/cnbiloop/cnbitkmat/mtpath');\
	end;\
	status0 = mtpath_include('%s');\
	if(status0 == false);\
		status1 = mtpath_include('%s');\
	end;\
	if(status0 == false && status1 == false);\
		disp('Error: includes failed');\
		exit;\
	end;\
catch e;\
	disp(['Exception: ' exception.message ]);\
	disp(exception);\
	disp(exception.stack);\
	disp('Killing Matlab...');\
	exit;\
end;\n"

#define CLMATLAB_ADDPATH "\
try;\
	status = exist('%s', 'dir');\
	if(status == false);\
		exit;\
	else;\
		addpath('%s');\
	end;\
catch e;\
	disp(['Exception: ' exception.message ]);\
	disp(exception);\
	disp(exception.stack);\
	disp('Killing Matlab...');\
	exit;\
end\n"

#define CLMATLAB_CWD "\
try;\
	status = exist('%s', 'dir');\
	cd('%s');\
	if(status == false);\
		exit;\
	end;\
catch e;\
	disp(['Exception: ' exception.message ]);\
	disp(exception);\
	disp(exception.stack);\
	disp('Killing Matlab...');\
	exit;\
end\n"

#define CLMATLAB_EXEC	"\
try;\
	%s;\
catch e;\
	disp(['Exception: ' exception.message ]);\
	disp(exception);\
	disp(exception.stack);\
	disp('Killing Matlab...');\
	exit;\
end\n"

ClMatlab::ClMatlab(void) : CcProcess(ClLoopConfig::matlabBinary, true, true) {
	std::string timestamp;
	CcTime::Daystamp(&timestamp);
	this->_logfile = CcCore::GetDirectoryTmp() + timestamp + "." + "cl_matlab.txt";
	CcLogConfigS("Matlab log file: " << this->_logfile);
}

ClMatlab::~ClMatlab(void) {
}

void ClMatlab::Exec(void) { 
	if(ClLoopConfig::matlabVariant.empty() == true) {
		execlp(this->_cmd.c_str(), "-nodesktop", "-nojvm", "-nosplash", 
				"-logfile", this->_logfile.c_str(), "2>&1", NULL);
	} else {
		std::string tvariant = "v=" + ClLoopConfig::matlabVariant;
		execlp(this->_cmd.c_str(), tvariant.c_str(), "-nodesktop", "-nojvm",
				"-nosplash", "-logfile", this->_logfile.c_str(), "2>&1", NULL);
	}
}

void ClMatlab::AddPath(const std::string& path) {
	char buffer[2048];
	sprintf(buffer, CLMATLAB_ADDPATH, path.c_str(), path.c_str());

	CcLogDebugS("Adding path: " << buffer);
	CcProcess::Write(buffer);
}

void ClMatlab::Include(const std::string& path) {
	char buffer[2048];
	sprintf(buffer, CLMATLAB_INCLUDE1, path.c_str());
	
	CcLogDebugS("Including path: " << buffer);
	CcProcess::Write(buffer);
}

void ClMatlab::Include(const std::string& path0, const std::string& path1) {
	char buffer[2048];
	sprintf(buffer, CLMATLAB_INCLUDE2, path0.c_str(), path1.c_str());
	
	CcLogDebugS("Including path: " << buffer);
	CcProcess::Write(buffer);
}
		
void ClMatlab::Directory(const std::string& path) {
	char buffer[2048];
	sprintf(buffer, CLMATLAB_CWD, path.c_str(), path.c_str());
	CcLogDebugS("Changing directory: " << buffer);
	CcProcess::Write(buffer);
}

void ClMatlab::Exec(const std::string function) {
	CcLogFatal("FUCK EVERYBODY");
	char buffer[2048];
	sprintf(buffer, CLMATLAB_EXEC, function.c_str());
	CcLogDebugS("Executing: " << buffer);
	CcProcess::Write(buffer);
}

#endif
