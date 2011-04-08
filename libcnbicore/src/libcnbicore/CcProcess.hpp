/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCPROCESS_HPP 
#define CCPROCESS_HPP 

#include "CcBasic.hpp"
#include <string>
#include <unistd.h>

/*! \brief Process management by the means for fork() and exec*()
 */
class CcProcess : public CcObject {
	public:
		CcProcess(const std::string& cmd, bool write, bool read);
		virtual ~CcProcess(void);
		virtual pid_t Fork(void);
		virtual int Terminate(void);
		virtual int Kill(void);
		virtual int Write(const std::string& buffer);
		virtual int Write(const char* buffer);
		virtual int Read(char* buffer, size_t count = 2048);
		virtual bool IsAlive(void);
	protected:
		virtual void Exec(void);
		virtual pid_t Popen(int* input, int* output);	

	public:
		static const int ProcessRead = 0;
		static const int ProcessWrite = 1;
	private:
		bool _canread;
		bool _canwrite;
		int _fidWrite;
		int _fidRead;
		pid_t _pid;
	protected:
		std::string _cmd;
		std::stringstream _logstream;
};

#endif
