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

#ifndef CLMATLAB_HPP 
#define CLMATLAB_HPP 

#include <libcnbicore/CcBasic.hpp>
#include <libcnbicore/CcProcess.hpp>

class ClMatlab : public CcProcess {
	public:
		ClMatlab(const std::string& cmd = "matlab", 
				bool write = true, bool read = true);
		virtual ~ClMatlab(void);
		void AddPath(const std::string& path);
		void Include(const std::string& path0, const std::string& path1);
		void ChangeDirectory(const std::string& path);
		
		void Launch(const std::string function);
		void LaunchNDF(const std::string function,
				const std::string& pipename, 
				const CcAddress addressD,
				const CcAddress addressC);
	protected:
		virtual void Exec(void);
};

#endif
