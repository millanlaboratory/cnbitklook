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

#ifndef CLMATLAB_HPP 
#define CLMATLAB_HPP 

#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcProcess.hpp>

class ClMatlab : public CcProcess {
	public:
		ClMatlab(const std::string& cmd = "matlab", 
				bool write = true, bool read = true);
		virtual ~ClMatlab(void);
		void AddPath(const std::string& path);
		void Include(const std::string& path);
		void Include(const std::string& path0, const std::string& path1);
		void ChangeDirectory(const std::string& path);
		
		void Exec(const std::string function);
		void ExecNDF(const std::string function);
	protected:
		virtual void Exec(void);
};

#endif
