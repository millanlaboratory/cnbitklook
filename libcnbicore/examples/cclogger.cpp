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

#include <stdio.h>
#include "CcLog.hpp"
#include "CcTime.hpp"

int main(void) {
	CcCore::OpenLogger("ciao");

	std::string message;
	for(int i = 0; i < 1; i++) {
		CcTime::Datetime(&message);
		CcLogConfig(message);
		CcLogException(message);
		CcLogDebug(message);
		CcLogInfo(message); 
		CcLogWarning(message);
		CcLogError(message);
		CcLogFatal(message);
		CcLogFatal(std::string("Hello ").append("world"));
		
		CcLogFatalS("Test with vargs 1: " << 20 << " " << 2.01f);
		CcLogFatalS("Test with vargs 2: " << 'a' << " " << true);
		CcLogFatalS("Test with vargs 3: " << 0x304 << " " << 1.0f);
	}
	return 0;
}
