#include "libndf/ndf_types.h"
#include "libndf/ndf_codec.h"
#include "libndf/ndf_presets.h"
#include "libtranspipe/tp_namedpipe.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
	size_t wsize = 0;
	unsigned long int total_frames = 10000; //4294967295;
	
	ndf_frame frame;
	ndf_preset_biosemi_032_2048(&frame);
	//ndf_preset_gtec_016_2048(&frame);
	ndf_init(&frame);
	ndf_dump(&frame);
	
	tp_npipe pipe;
	tp_init(&pipe, "/tmp/ndf.example.ack");
	if(tp_create(&pipe) < 0)
		printf("Cannot create pipe\n");
	if(tp_openwrite(&pipe) <= 0)
		printf("Cannot open pipe for writing\n");
	tp_catchsigpipe();
	
	unsigned short int GDFLabel = 0;
	unsigned int i = 0, j = 0;
	float* eeg = malloc(frame.sizes.eeg_sample);
	float* exg = malloc(frame.sizes.exg_sample);
	int32_t* tri = malloc(frame.sizes.tri_sample);

	for(i = 0; i < frame.config.eeg_channels; i++)
		eeg[i] = (float)(10 + i);
	for(i = 0; i < frame.config.exg_channels; i++)
		exg[i] = (float)(100 + i);
	for(i = 0; i < frame.config.tri_channels; i++) {
			tri[i] = (int32_t)(666);
	}
	
	for(j = 0; j < frame.config.samples ; j++) {
		ndf_write_eeg_sample(&frame, eeg, j);
		ndf_write_exg_sample(&frame, exg, j);
		ndf_write_tri_sample(&frame, tri, j);
	}
	
	for(i = 0; i < frame.config.labels; i++) {
		GDFLabel = 700 + i; 
		ndf_set_label(&frame, (void*)&GDFLabel, i);
	}


	printf("Writing NDF ACK\n");
	wsize = tp_write(&pipe, frame.ack.buffer, frame.ack.bsize);
	for(i = 0; i < NDF_ACK_LENGHT; i++)
		printf("%d\n", frame.ack.buffer[i]);

	printf("Writing to pipe\n");
	for(i = 0; i < total_frames; i++) {
		ndf_timevaltic(&frame);
		ndf_set_fidx(&frame, i+1);
		ndf_tic(&frame);
		wsize = tp_write(&pipe, frame.data.buffer, frame.data.bsize);
		if(wsize <= 0) {
			printf("Cannot write pipe\n");
			break;
		}
		if(tp_receivedsigpipe() == 1) {
			printf("Broken pipe\n");
			break;
		}
	}
	
	if(tp_close(&pipe) < 0) {
		printf("Cannot close pipe\n");
		exit(1);
	}
	
	if(tp_remove(&pipe) != 0) {
		printf("Cannot remove pipe\n");
		exit(1);
	}
	
	printf("Going down\n");
	ndf_free(&frame);

	return 0;
}
