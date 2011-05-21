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

#ifndef CLPROCLIENT_HPP 
#define CLPROCLIENT_HPP 

#include "ClClient.hpp"
#include "ClProLang.hpp"

class ClProClient : public ClClient {
	public:
		bool Connect(const CcAddress address = "127.0.0.1:9100");
		int Fork(int* rpid);
		int ForkAndCheck(int* rpid, double waitms = 5000.00f);
		int Exec(const int pid, const std::string& call);
		int IsAlive(const int pid);
		int Terminate(const int pid);
		int Directory(const int pid, const std::string& path);
		int Include(const int pid, const std::string& path);
		int Include(const int pid, const std::string& path0, 
				const std::string& path1);
		int IncludeNDF(const int pid);
		bool Check(const int pid, double waitms = 2500.00f);
	private:
	protected:

	public:
	private:
		ClProLang _language;
	protected:
};

#endif
