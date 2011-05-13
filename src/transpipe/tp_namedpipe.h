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

#ifndef TPNAMEDPIPE_H
#define TPNAMEDPIPE_H

#include <stdlib.h>

#define TP_FNSIZE	512
/* TODO 2010-11-18  Michele Tavella <michele.tavella@epfl.ch>
 * When events are catched, a failure in write() will return 
 * TP_BROKENS bytes written.
 * TP_BROKENS is the upper limit of size_t. This needs to be tested 
 * because I found out by chance.
 */
#define TP_BROKENS	((size_t)-1)

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct tpnpipe_struct {
	int fid;
	char filename[TP_FNSIZE];
} tp_npipe;

void tp_init(tp_npipe* pipe, const char* filename);
int tp_create(const tp_npipe* pipe);
int tp_remove(const tp_npipe* pipe);
int tp_openread(tp_npipe* pipe);
int tp_openwrite(tp_npipe* pipe);
int tp_close(const tp_npipe* pipe);
size_t tp_write(const tp_npipe* pipe, const void* buffer, const size_t bsize);
size_t tp_read(const tp_npipe* pipe, void* buffer, const size_t bsize);
void tp_catchsigpipe(void);
int tp_receivedsigpipe(void);
int tp_exist(const tp_npipe* pipe);

#ifdef __cplusplus
}
#endif

#endif
