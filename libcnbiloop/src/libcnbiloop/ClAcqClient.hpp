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

#ifndef CLACQCLIENT_HPP 
#define CLACQCLIENT_HPP 

#include "ClClient.hpp"
#include "ClAcqLang.hpp"

class ClAcqClient : public ClClient {
	public:
		virtual bool Connect(const CcAddress address = "127.0.0.1:9000");
		virtual int AddLabelGDF(const GDFEvent label);
		virtual int AddLabelLPT(const HWTrigger label);
		virtual int AddLabelTXT(const std::string& label);
		virtual int OpenXDF(const std::string& filegdf, const std::string& filelog,
				const std::string& linelog);
		virtual int CloseXDF(void);
	private:
	protected:

	public:
	private:
		ClAcqLang _language;
	protected:
};

#endif
