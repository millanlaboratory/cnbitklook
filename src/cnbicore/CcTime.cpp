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

#ifndef CCTIME_CPP
#define CCTIME_CPP

#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/select.h>
#include <time.h>
#include <cstdio>

#include "CcTime.hpp"

void CcTime::Timestamp(std::string* timestamp) {
	char temp[128];

	struct tm *tmp = NULL; 
	time_t t;

	t = time(NULL);
	tmp = localtime(&t);
	sprintf(temp, "%04d%02d%02d%02d%02d%02d", 
			tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, 
			tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	timestamp->clear();
	timestamp->assign(temp);
}

void CcTime::Daystamp(std::string* timestamp) {
	char temp[128];

	struct tm *tmp = NULL; 
	time_t t;

	t = time(NULL);
	tmp = localtime(&t);
	sprintf(temp, "%04d%02d%02d-%02d%02d%02d", 
			tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, 
			tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	timestamp->clear();
	timestamp->assign(temp);
}

void CcTime::Datetime(std::string* timestamp) {
	char temp[128];

	struct tm *tmp = NULL; 
	time_t t;

	t = time(NULL);
	tmp = localtime(&t);
	sprintf(temp, "%04d.%02d.%02d-%02d:%02d:%02d", 
			tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, 
			tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	timestamp->clear();
	timestamp->assign(temp);
}
		
void CcTime::Sleep(double ms) {
	/* 2009-11-02  Michele Tavella <michele.tavella@epfl.ch>
	 * Paranoia mode on 
	 */
	timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = (long)1000*ms;

	select(0, NULL, NULL, NULL, &tm);
}

double CcTime::Sleep(double ms0, double ms1) {
	/* 2009-11-02  Michele Tavella <michele.tavella@epfl.ch>
	 * Paranoia mode on 
	 */
	//assert(ms1 > ms0);
	// TODO
	srand(time(NULL));
	double ms = rand()*(ms1-ms0)/RAND_MAX + ms0;
	CcTime::Sleep(ms);
	return ms;
}

void CcTime::Tic(CcTimeValue* tic) {
	gettimeofday(tic, NULL);
}

double CcTime::Toc(CcTimeValue* tic) {
	CcTimeValue toc;
	gettimeofday(&toc, NULL);

	return double((toc.tv_sec  - tic->tv_sec)*1000000 + 
			toc.tv_usec - tic->tv_usec)/1000;
}
		
void CcTime::Partial(CcTimeValue* tic, std::string step) {
	double elapsed = CcTime::Toc(tic);
	CcTime::Tic(tic);
	printf("[CcTime::Dump] %s: %f\n", step.c_str(), elapsed);
}
		
CcTimeValueHPT CcTime::HPT(void) {
	unsigned a, d;
	__asm__ volatile("rdtsc" : "=a" (a), "=d" (d));
	return ((unsigned long long)a) | (((unsigned long long)d) << 32);;
}

#endif
