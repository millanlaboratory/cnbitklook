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

#ifndef CLNAMESCLIENT_HPP 
#define CLNAMESCLIENT_HPP 

#include "ClClient.hpp"
#include "ClNamesLang.hpp"

class ClNamesClient : public ClClient {
	public:
		virtual bool Connect(const CcAddress address = "127.0.0.1:8000");
		virtual int Query(const std::string& name, CcAddress* address);
		virtual int Set(const std::string& name, CcAddress address);
		virtual int Unset(const std::string& name);
		virtual CcAddress Query(const std::string& name);
	private:
	protected:

	public:
	private:
		ClNamesLang _language;
	protected:
};

#endif
