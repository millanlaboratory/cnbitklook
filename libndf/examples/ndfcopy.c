/*
	Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libndf library

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

#include "ndf_codec.h"
#include "ndf_presets.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

int main(void) {
	unsigned int i = 0;
	ndf_frame frame0;
	ndf_frame frame1;
	ndf_frame frame2;
	ndf_clear(&frame0);
	ndf_clear(&frame1);
	ndf_clear(&frame2);

	ndf_preset_gtec_016_0512(&frame0);
	ndf_init(&frame0);
	ndf_dump(&frame0);
	
	unsigned int label = 101;
	ndf_set_label(&frame0, &label, 1);

	ndf_preset_gtec_016_0512(&frame1);
	ndf_init(&frame1);
	ndf_dump(&frame1);
	
	memcpy(frame1.data.buffer, frame0.data.buffer, frame0.data.bsize);

	label = 0;
	ndf_get_label(&frame1, &label, 1);
	printf("And the label is: %d\n", label);



	ndf_initack(&frame2, &(frame0.ack));
	for(i = 0; i < NDF_ACK_LENGHT; i++)
		printf("Pos. %d = %d/%d\n", 
				i, frame0.ack.buffer[i], frame2.ack.buffer[i]);
	
	ndf_dump(&frame0);
	ndf_dump(&frame2);

	ndf_free(&frame0);
	ndf_free(&frame1);
	ndf_free(&frame2);
	return 0;
}
