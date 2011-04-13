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

#ifndef NDFCODEC_C
#define NDFCODEC_C

#include "ndf_codec.h"
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

void ndf_clear(ndf_frame* frame) {
	memset(frame, 0, sizeof(ndf_frame));
}

void ndf_init(ndf_frame* frame) {
	// Fill up type sizes
	frame->sizes.tim_type = ndf_sizeof(frame->types.tim);
	frame->sizes.lbl_type = ndf_sizeof(frame->types.lbl);
	frame->sizes.eeg_type = ndf_sizeof(frame->types.eeg);
	frame->sizes.exg_type = ndf_sizeof(frame->types.exg);
	frame->sizes.tri_type = ndf_sizeof(frame->types.tri);

	// Fill up data sizes
	frame->sizes.eeg_sample = 
		frame->config.eeg_channels * frame->sizes.eeg_type;
	frame->sizes.exg_sample = 
		frame->config.exg_channels * frame->sizes.exg_type;
	frame->sizes.tri_sample = 
		frame->config.tri_channels * frame->sizes.tri_type;
	
	frame->sizes.frame = frame->config.samples * 
		(frame->sizes.tri_sample + 
		 frame->sizes.eeg_sample +
		 frame->sizes.exg_sample);
	
	frame->sizes.labels = 
		frame->config.labels * frame->sizes.lbl_type;
	
	// Compute size of buffer
	frame->data.bsize = 
		frame->sizes.tim_type +
		sizeof(uint64_t) + 
		frame->sizes.labels +
		frame->sizes.frame;
	frame->data.buffer = malloc((size_t)frame->data.bsize);
	memset(frame->data.buffer, 0, frame->data.bsize);
	
	// Fillup offsets
	frame->offset.timestamp = 
		(void*)(frame->data.buffer);
	frame->offset.fidx = 
		(void*)(frame->data.buffer + frame->sizes.tim_type);
	frame->offset.labels = 
		(void*)(frame->offset.fidx + sizeof(uint64_t));

	frame->offset.eeg = 
		(void*)(frame->offset.labels + frame->sizes.labels);
	frame->offset.exg = 
		(void*)(frame->offset.eeg + frame->config.samples*frame->sizes.eeg_sample);
	frame->offset.tri = 
		(void*)(frame->offset.exg + frame->config.samples*frame->sizes.exg_sample);
	
	if(frame->config.exg_channels == 0)
		frame->offset.exg = NULL;
	if(frame->config.eeg_channels == 0)
		frame->offset.eeg = NULL;
	if(frame->config.tri_channels == 0)
		frame->offset.tri = NULL;

	// Zero index
	ndf_set_fidx(frame, 0);
	frame->extra.lidx = -1;

	frame->ack.buffer[0] = frame->config.id;
	frame->ack.buffer[1] = frame->config.sf;
	frame->ack.buffer[2] = frame->config.labels;
	frame->ack.buffer[3] = frame->config.samples;
	frame->ack.buffer[4] = frame->config.eeg_channels;
	frame->ack.buffer[5] = frame->config.exg_channels;
	frame->ack.buffer[6] = frame->config.tri_channels;
	frame->ack.buffer[7] = frame->types.tim;
	frame->ack.buffer[8] = frame->types.eeg;
	frame->ack.buffer[9] = frame->types.exg;
	frame->ack.buffer[10] = frame->types.tri;
	frame->ack.buffer[11] = frame->types.lbl;
	frame->ack.bsize = NDF_ACK_LENGHT*sizeof(unsigned int);
}

void ndf_initack(ndf_frame* frame, ndf_ack* ack) {
	frame->config.id 				= ack->buffer[0];
	frame->config.sf 				= ack->buffer[1];
	frame->config.labels 			= ack->buffer[2];
	frame->config.samples 			= ack->buffer[3];
	frame->config.eeg_channels 		= ack->buffer[4];
	frame->config.exg_channels 		= ack->buffer[5];
	frame->config.tri_channels 		= ack->buffer[6];
	frame->types.tim 				= ack->buffer[7];
	frame->types.eeg 				= ack->buffer[8];
	frame->types.exg 				= ack->buffer[9];
	frame->types.tri 				= ack->buffer[10];
	frame->types.lbl 				= ack->buffer[11];
	ndf_init(frame);
}

void ndf_free(ndf_frame* frame) {
	if(frame->data.buffer == NULL)
		return;
	free(frame->data.buffer);
	frame->data.buffer = NULL;
}

void ndf_dump(const ndf_frame* frame) {
	printf("[ndf_dump] Frame informations:\n");
	printf(" + Config:\n");
	printf(" |- Id:             %u\n", frame->config.id);
	printf(" |- Sf:             %u Hz\n", frame->config.sf);
	printf(" |- Channels EEG:   %u\n", frame->config.eeg_channels);
	printf(" |- Channels EXG:   %u\n", frame->config.exg_channels);
	printf(" |- Channels TRI:   %u\n", frame->config.tri_channels);
	printf(" |- Labels:         %u\n", frame->config.labels);
	printf(" `- Samples:        %u\n", frame->config.samples);
	printf(" + Types:\n");
	printf(" |- Timestamp:      %lu bytes\n", frame->sizes.tim_type);
	printf(" |- Label:          %lu bytes\n", frame->sizes.lbl_type);
	printf(" |- EEG:            %lu bytes\n", frame->sizes.eeg_type);
	printf(" |- EXG:            %lu bytes\n", frame->sizes.exg_type);
	printf(" `- TRI:            %lu bytes\n", frame->sizes.tri_type);
	printf(" + Data:\n");
	printf(" |- EEG sample:     %lu bytes\n", frame->sizes.eeg_sample);
	printf(" |- EXG sample:     %lu bytes\n", frame->sizes.exg_sample);
	printf(" |- TRI sample:     %lu bytes\n", frame->sizes.tri_sample);
	printf(" |- Frame:          %lu bytes\n", frame->sizes.frame);
	printf(" `- Labels:         %lu bytes\n", frame->sizes.labels);
	printf(" + Pointers:\n");
	printf(" |- Structure:      %p\n", frame);
	printf(" |- Timestamp:      %p\n", frame->offset.timestamp);
	printf(" |- Index:          %p\n", frame->offset.fidx);
	printf(" |- Labels:         %p\n", frame->offset.labels);
	printf(" |- EEG frame:      %p\n", frame->offset.eeg);
	printf(" |- EXG frame:      %p\n", frame->offset.exg);
	printf(" `- TRI frame:      %p\n", frame->offset.tri);
	printf(" + Buffer:\n");
	printf(" |- Pointer:        %p\n", frame->data.buffer);
	printf(" `- Size:           %lu bytes\n", frame->data.bsize);
}

void* ndf_set_timestamp(ndf_frame* frame, const void* timestamp) {
	return memcpy(frame->data.buffer, timestamp, frame->sizes.tim_type);
}

void* ndf_get_timestamp(const ndf_frame* frame, void* timestamp) {
	return memcpy(timestamp, frame->data.buffer, frame->sizes.tim_type);
}

void* ndf_set_fidx(ndf_frame* frame, const int idx) {
	return memcpy(frame->offset.fidx, &idx, sizeof(int));
}
 
uint64_t ndf_get_fidx(const ndf_frame* frame) {
	int idx;
	memcpy(&idx, frame->offset.fidx, sizeof(int));
	return idx;
}

uint64_t ndf_inc_fidx(ndf_frame* frame) {
	int idx = ndf_get_fidx(frame);
	++idx;
	ndf_set_fidx(frame, idx);
	return idx;
}

uint64_t ndf_dex_fidx(ndf_frame* frame) {
	int idx = ndf_get_fidx(frame);
	--idx;
	ndf_set_fidx(frame, idx);
	return idx;
}

void* ndf_set_label(ndf_frame* frame, void* label, const unsigned int index) {
	if(index >= frame->config.labels)
		return NULL;
	frame->extra.lidx = index;
	size_t offset = index*frame->sizes.lbl_type;
	return memcpy(frame->offset.labels + offset, label, frame->sizes.lbl_type);
}

void* ndf_get_label(const ndf_frame* frame, void* label, unsigned int index) {
	if(index >= frame->config.labels)
		return NULL;
	size_t offset = index*frame->sizes.lbl_type;
	return memcpy(label, frame->offset.labels + offset, frame->sizes.lbl_type);
}

void* ndf_add_label(ndf_frame* frame, void* label) {
	if(frame->extra.lidx == frame->config.labels - 1)
		return NULL;
	size_t offset = (++frame->extra.lidx)*frame->sizes.lbl_type;
	return memcpy(frame->offset.labels + offset, label, frame->sizes.lbl_type);
}

void* ndf_clear_labels(ndf_frame* frame) {
	size_t sizet = frame->sizes.lbl_type * frame->config.labels;
	frame->extra.lidx = -1;
	return memset(frame->offset.labels, 0, sizet);
}

void* ndf_eeg(const ndf_frame* frame) {
	return frame->offset.eeg;
}

void* ndf_exg(const ndf_frame* frame) {
	return frame->offset.exg;
}

void* ndf_tri(const ndf_frame* frame) {
	return frame->offset.tri;
}

int ndf_timevaltic(ndf_frame* frame) {
	if(frame->sizes.tim_type != ndf_sizeof(NDF_TIMEVAL))
		return -1;

	struct timeval tic;
	gettimeofday(&tic, 0);
	ndf_set_timestamp(frame, &tic);
	return 0;
}

double ndf_timevaltoc(const ndf_frame* frame) {
	if(frame->sizes.tim_type != ndf_sizeof(NDF_TIMEVAL))
		return -1;
	struct timeval tic;
	struct timeval toc;
	gettimeofday(&toc, 0);
	ndf_get_timestamp(frame, &tic);
	return (double)((toc.tv_sec  - tic.tv_sec)*1000000 + 
			toc.tv_usec - tic.tv_usec)/1000;
}

void ndf_doubletimetic(ndf_frame* frame) {
	assert(frame->sizes.tim_type == ndf_sizeof(NDF_DOUBLETIME));
	
	struct timeval tic;
	gettimeofday(&tic, 0);
	
	double ticv = (double)(tic.tv_sec*1000000 + tic.tv_usec)/1000;
	ndf_set_timestamp(frame, &ticv);
}

double ndf_doubletimetoc(const ndf_frame* frame) {
	if(frame->sizes.tim_type != ndf_sizeof(NDF_DOUBLETIME))
		return -1;

	double ticv, tocv;
	struct timeval toc;
	gettimeofday(&toc, 0);
	ndf_get_timestamp(frame, &ticv);
	tocv = (double)(toc.tv_sec*1000000 + toc.tv_usec)/1000;
	return tocv - ticv;
}

void* ndf_read_eeg_sample(const ndf_frame* frame, void* sample, const unsigned int index) { 
	size_t offset = frame->sizes.eeg_sample * index;
	return memcpy(sample, (void*)(frame->offset.eeg + offset), frame->sizes.eeg_sample);
}

void* ndf_write_eeg_sample(ndf_frame* frame, const void* sample, const unsigned int index) { 
	size_t offset = frame->sizes.eeg_sample * index;
	return memcpy(frame->offset.eeg + offset, sample, frame->sizes.eeg_sample);
}

void* ndf_read_exg_sample(const ndf_frame* frame, void* sample, const unsigned int index) { 
	size_t offset = frame->sizes.exg_sample * index;
	return memcpy(sample, frame->offset.exg + offset, frame->sizes.exg_sample);
}

void* ndf_write_exg_sample(ndf_frame* frame, const void* sample, const unsigned int index) { 
	size_t offset = frame->sizes.exg_sample * index;
	return memcpy(frame->offset.exg + offset, sample, frame->sizes.exg_sample);
}

void* ndf_read_tri_sample(const ndf_frame* frame, void* sample, const unsigned int index) { 
	size_t offset = frame->sizes.tri_sample * index;
	return memcpy(sample, frame->offset.tri + offset, frame->sizes.tri_sample);
}

void* ndf_write_tri_sample(ndf_frame* frame, const void* sample, 
		const unsigned int index) { 
	size_t offset = frame->sizes.tri_sample * index;
	return memcpy(frame->offset.tri + offset, sample, frame->sizes.tri_sample);
}

void* ndf_read_eeg_frame(const ndf_frame* frame, void* data) { 
	return memcpy(data, frame->offset.eeg, 
			frame->config.samples*frame->sizes.eeg_sample);
}

void* ndf_write_eeg_frame(ndf_frame* frame, const void* data) { 
	return memcpy(frame->offset.eeg, data, 
			frame->config.samples*frame->sizes.eeg_sample);
}

void* ndf_read_exg_frame(const ndf_frame* frame, void* data) { 
	return memcpy(data, frame->offset.exg, 
			frame->config.samples*frame->sizes.exg_sample);
}

void* ndf_write_exg_frame(ndf_frame* frame, const void* data) { 
	return memcpy(frame->offset.exg, data, 
			frame->config.samples*frame->sizes.exg_sample);
}

void* ndf_read_tri_frame(const ndf_frame* frame, void* data) { 
	return memcpy(data, frame->offset.tri,
			frame->config.samples*frame->sizes.tri_sample);
}

void* ndf_write_tri_frame(ndf_frame* frame, const void* data) { 
	return memcpy(frame->offset.tri, data, 
			frame->config.samples*frame->sizes.tri_sample);
}

void ndf_tic(ndf_frame* frame) {
	switch(frame->types.tim) {
		case NDF_DOUBLETIME:
			ndf_doubletimetic(frame);
			break;
		case NDF_TIMEVAL:
			ndf_timevaltic(frame);
			break;
	}
}

double ndf_toc(const ndf_frame* frame) {
	switch(frame->types.tim) {
		case NDF_DOUBLETIME:
			return ndf_doubletimetoc(frame);
		case NDF_TIMEVAL:
			return ndf_timevaltoc(frame);
	}
	return 0;
}

void ndf_print_labels(ndf_frame* frame) {
	unsigned short int lbl = 0;
	unsigned int i = 0;
	printf("[ndf_print_labels] %8.8lu/%3.3d:", 
			ndf_get_fidx(frame),
			frame->config.labels);
	for(i = 0; i < frame->config.labels; i++, lbl = 0) {
		ndf_get_label(frame, (void*)&lbl, i);
		lbl == 0 ? printf("  - ") : printf(" %3.3d", lbl);
	}
	printf("\n");
}
#endif
