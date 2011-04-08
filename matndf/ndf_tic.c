/*
   Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

   This file is part of matndf

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

#include "mex.h"
#include <sys/time.h>

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	if (nrhs != 0) 
        mexErrMsgTxt("[ndf_tic] No argument expected.");
	else if(nlhs > 1)
        mexErrMsgTxt("[ndf_tic] Too many output arguments");

	double* dt;
	struct timeval tic;
	double ticv;
	
	gettimeofday(&tic, 0);
	ticv = (double)(tic.tv_sec*1000000 + tic.tv_usec)/1000; 
	
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	dt = mxGetPr(plhs[0]);
	*dt = ticv;
}
