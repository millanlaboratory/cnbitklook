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

#ifndef NDFCODEC_H
#define NDFCODEC_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdlib.h>
#include <stdint.h>
#include "ndf_types.h"

#define NDF_ACK_LENGHT	12
#define NDF_ACK_SIZET	NDF_ACK_LENGHT * sizeof(unsigned int)

typedef struct ndf_ack_struct {
	/* The message contains the following informations:
	 * 0	Preset ID				From ndf_config
	 * 1	Sampling rate				"
	 * 2	Number of labels			"
	 * 3	Number of samples			"
	 * 4	Number of EEG channels		"
	 * 5	Number of EXG channels		"
	 * 6	Number of TRI channels		"
	 * 7	Timestamp type			From ndf_types
	 * 8    EEG sample type				"
	 * 9    EXG sample type				"
	 * 10   TRI sample type				"
	 * 11 	Label type					"
	 */
	unsigned int buffer[NDF_ACK_LENGHT];
	size_t bsize;
} ndf_ack;

typedef struct ndf_data_struct {
	void* buffer;			// Buffer
	size_t bsize;			// Buffer size
} ndf_data;

typedef struct ndf_types_struct {
	ndf_typeid tim;			// Timestamp type
	ndf_typeid eeg;			// Sample type
	ndf_typeid exg;			// Sample type
	ndf_typeid tri;			// Sample type
	ndf_typeid lbl;			// Label type
} ndf_types;

typedef struct ndf_size_struct {
	size_t tim_type;		// Timestamp type
	size_t eeg_type;		// EEG sample type
	size_t exg_type;		// EXG sample type
	size_t tri_type;		// TRI sample type
	size_t lbl_type;		// Label type
	size_t eeg_sample;		// Sizeof EEG sample
	size_t exg_sample;		// Sizeof EEG sample
	size_t tri_sample;		// Sizeof EEG sample
	size_t frame;			// Sizeof frame
	size_t labels;			// Sizeof labels
} ndf_size;

typedef struct ndf_offsets_struct {
	void* timestamp;	// Pointer to index area
	void* fidx;			// Pointer to fidx area
	void* labels;		// Pointer to labels area
	void* eeg;			// Pointer to EEG frame area
	void* exg;			// Pointer to EXG frame area
	void* tri;			// Pointer to TRI frame area
} ndf_offset;

typedef struct ndf_config_struct {
	unsigned int id;			// Preset ID
	unsigned int sf;			// Sampling rate
	unsigned int labels;		// Number of labels
	unsigned int samples;		// Number of samples
	unsigned int eeg_channels;	// Number of EEG channels
	unsigned int exg_channels;	// Number of EXG channels
	unsigned int tri_channels;	// Number of TRI channels
} ndf_config;
	
typedef struct ndf_extra_struct {
	int lidx; 			// Index of last added label
} ndf_extra;

typedef struct ndf_frame_struct {
	ndf_offset offset;
	ndf_config config;
	ndf_size sizes;
	ndf_data data;
	ndf_types types;
	ndf_ack ack;
	ndf_extra extra;
} ndf_frame;

void ndf_clear(ndf_frame* frame);
void ndf_init(ndf_frame* frame);
void ndf_initack(ndf_frame* frame, ndf_ack* ack);
void ndf_free(ndf_frame* frame);
void ndf_dump(const ndf_frame* frame);
void* ndf_set_timestamp(ndf_frame* frame, const void* timestamp);
void* ndf_get_timestamp(const ndf_frame* frame, void* timestamp);
void* ndf_set_fidx(ndf_frame* frame, const int idx);
uint64_t ndf_inc_fidx(ndf_frame* frame);
uint64_t ndf_dex_fidx(ndf_frame* frame);
uint64_t ndf_get_fidx(const ndf_frame* frame);
void* ndf_add_label(ndf_frame* frame, void* label);
void* ndf_set_label(ndf_frame* frame, void* label, unsigned int index);
void* ndf_get_label(const ndf_frame* frame, void* label, unsigned int index);
void* ndf_clear_labels(ndf_frame* frame);
void* ndf_eeg(const ndf_frame* frame);
void* ndf_exg(const ndf_frame* frame);
void* ndf_tri(const ndf_frame* frame);
void ndf_tic(ndf_frame* frame);
double ndf_toc(const ndf_frame* frame);
int ndf_timevaltic(ndf_frame* frame);
double ndf_timevaltoc(const ndf_frame* frame);
void ndf_doubletimetic(ndf_frame* frame);
double ndf_doubletimetoc(const ndf_frame* frame);
void* ndf_read_eeg_sample(const ndf_frame* frame, void* sample, 
		const unsigned int index); 
void* ndf_write_eeg_sample(ndf_frame* frame, const void* sample, 
		const unsigned int index); 
void* ndf_read_exg_sample(const ndf_frame* frame, void* sample, 
		const unsigned int index); 
void* ndf_write_exg_sample(ndf_frame* frame, const void* sample, 
		const unsigned int index); 
void* ndf_read_tri_sample(const ndf_frame* frame, void* sample, 
		const unsigned int index); 
void* ndf_write_tri_sample(ndf_frame* frame, const void* sample, 
		const unsigned int index); 
void* ndf_read_eeg_frame(const ndf_frame* frame, void* data);
void* ndf_write_eeg_frame(ndf_frame* frame, const void* data);
void* ndf_read_exg_frame(const ndf_frame* frame, void* data);
void* ndf_write_exg_frame(ndf_frame* frame, const void* data);
void* ndf_read_tri_frame(const ndf_frame* frame, void* data);
void* ndf_write_tri_frame(ndf_frame* frame, const void* data);
void ndf_print_labels(ndf_frame* frame);

#ifdef __cplusplus
}
#endif

#endif
