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

#ifndef CCTIME_HPP
#define CCTIME_HPP

#include "CcObject.hpp"
#include <string>
#include <sys/time.h>

typedef struct timeval CcTimeValue;
typedef unsigned long long int CcTimeValueHPT;

/* 2009-11-02  Michele Tavella <michele.tavella@epfl.ch>
 * Once namespaces will be introduced, this will become one 
 * of them
 */
/*! \brief Time-handling functionalities
 *
 * Mainly static methods for time management in LibCNBICore. 
 * Please use these methods in your code so that code portability is simplified.
 */
class CcTime : public CcObject {
	public:
		static void Timestamp(std::string* timestamp);
		static void Daystamp(std::string* timestamp);
		static void Datetime(std::string* timestamp);
		static void Sleep(double ms);
		static double Sleep(double ms0, double ms1);
		static void Tic(CcTimeValue* tic);
		static double Toc(CcTimeValue* tic);
		static void Partial(CcTimeValue* tic, std::string step = "Unknown");

		/* 2009-11-28  Michele Tavella <michele.tavella@epfl.ch>
		 * Copy and paste from:
		 *  http://www.cs.wm.edu/~kearns/001lab.d/rdtsc.html
		 */
		static CcTimeValueHPT HPT(void);
};

#endif
