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

#include <ndf/ndf_codec.h>
#include <ndf/ndf_presets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

int main(void) {
	ndf_frame frame;
	ndf_clear(&frame);
	ndf_preset_tiny(&frame);
	ndf_init(&frame);
	ndf_dump(&frame);

	unsigned short int GDFLabel = 0;
	unsigned int i = 0, j = 0, k = 0;
	double* eeg = malloc(frame.sizes.eeg_sample);
	double* exg = malloc(frame.sizes.exg_sample);
	int32_t* tri = malloc(frame.sizes.tri_sample);
	double dt; 

	for(i = 0; i < frame.config.eeg_channels; i++)
		eeg[i] = (double)(10 + i);
	for(i = 0; i < frame.config.exg_channels; i++)
		exg[i] = (double)(100 + i);
	for(i = 0; i < frame.config.tri_channels; i++)
		tri[i] = (double)(1000 + i);
	
	for(i = 0; i < frame.config.labels; i++) {
		GDFLabel = 700 + i; 
		ndf_set_label(&frame, (void*)&GDFLabel, i);
	}

	printf("Labels:");
	for(i = 0; i < frame.config.labels; i++) {
		GDFLabel = 0;
		ndf_get_label(&frame, (void*)&GDFLabel, i);
		printf(" %d", GDFLabel);
	}
	printf("\n");

	for(j = 0; j < frame.config.samples ; j++) {
		ndf_write_eeg_sample(&frame, eeg, j);
		ndf_write_exg_sample(&frame, exg, j);
		ndf_write_tri_sample(&frame, tri, j);
	}

	for(j = 0; j < frame.config.samples ; j++) {
		ndf_read_eeg_sample(&frame, eeg, j);
		printf("EEG %3d: ", j);
		for(k = 0; k < frame.config.eeg_channels; k++)
			printf("%3.0f ", eeg[k]);
		printf("\n");
	}
	for(j = 0; j < frame.config.samples ; j++) {
		ndf_read_exg_sample(&frame, exg, j);
		printf("EXG %3d: ", j);
		for(k = 0; k < frame.config.exg_channels; k++) 
			printf("%3.0f ", exg[k]);
		printf("\n");
	}
	for(j = 0; j < frame.config.samples ; j++) {
		ndf_read_tri_sample(&frame, tri, j);
		printf("TRI %3d: ", j);
		for(k = 0; k < frame.config.tri_channels; k++) 
			printf("%3.0d ", tri[k]);
		printf("\n");
	}
	
	ndf_tic(&frame);
	for(i = 1; i < 5; i++) {
		dt = ndf_toc(&frame);
		printf("dt=%.3f\n", dt);
		usleep(15*1000);
		sleep(1);
	}

	/*
	struct timeval tic;
	struct timeval toc;
	gettimeofday(&tic, 0);
	
	ndf_set_timestamp(&frame, &tic);
	ndf_get_timestamp(&frame, &toc);

	double dt0 = (double)((toc.tv_sec  - tic.tv_sec)*1000000 + 
			toc.tv_usec - tic.tv_usec)/1000;
	printf("dt=%8.2f/idx=%lu\n", dt0, ndf_get_fidx(&frame));

	for(i = 0; i <= 2; i++) {
		printf("dt=%8.2f/idx=%lu\n", 
				ndf_timevaltoc(&frame), 
				ndf_inc_fidx(&frame));
		sleep(1);
	}
	*/

	ndf_free(&frame);
	free(eeg);

	return 0;
}

