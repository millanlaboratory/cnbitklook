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
#include <stdio.h>
#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcLog.hpp>
#include <cnbicore/CcTime.hpp>

int main(void) {
	CcCore::OpenLogger("ciao");

	std::string message("ciao");
	for(int i = 0; i < 1; i++) {
		CcTime::Datetime(&message);
		CcLogConfig(message);
		CcLogException(message);
		CcLogDebug(message);
		CcLogInfo(message); 
		CcLogWarning(message);
		CcLogError(message);
		CcLogFatal(message);
		
		CcLogFatalS("Test with vargs 1: " << 20 << " " << 2.01f);
		CcLogErrorS("Test with vargs 2: " << 'a' << " " << true);
		CcLogWarningS("Test with vargs 3: " << 0x304 << " " << 1.0f);
	}
	CcCore::Exit(0);	
}
