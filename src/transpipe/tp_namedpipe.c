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

#ifndef TPNAMEDPIPE_C
#define TPNAMEDPIPE_C

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include "tp_namedpipe.h"

int fidbrokenpipe = 0;

void tp_init(tp_npipe* pipe, const char *filename) {
	assert(strlen(filename) <= TP_FNSIZE);
	pipe->fid = 0;
	strcpy(pipe->filename, filename);
}

// Returns < 0 on error
int tp_create(const tp_npipe* pipe) {
	return mkfifo(pipe->filename, S_IRUSR | S_IWUSR | S_IFIFO);
}

// Returns 0 on success
int tp_remove(const tp_npipe* pipe) {
	return remove(pipe->filename);
}

// Retuns <= 0 on error
int tp_openread(tp_npipe* pipe) {
	pipe->fid = open(pipe->filename, O_RDONLY);
	fcntl(pipe->fid, F_SETFD, fcntl(pipe->fid, F_GETFD)|FD_CLOEXEC);
	return pipe->fid;
}

// Retuns <= 0 on error
int tp_openwrite(tp_npipe* pipe) {
	pipe->fid = open(pipe->filename, O_WRONLY);
	return pipe->fid;
}

// Retuns -1 on error
int tp_close(const tp_npipe* pipe) {
	return close(pipe->fid);
}

// Retuns bytes written
size_t tp_write(const tp_npipe* pipe, const void* buffer, const size_t bsize) {
	return write(pipe->fid, buffer, bsize);
}

// Retuns bytes read
size_t tp_read(const tp_npipe* pipe, void* buffer, const size_t bsize) {
	return read(pipe->fid, buffer, bsize);
}

void sigpipe_handler(int signum) {
	fidbrokenpipe = 1;
}

void tp_catchsigpipe(void) {
	signal(SIGPIPE, sigpipe_handler);
}

int tp_receivedsigpipe(void) {
	if(fidbrokenpipe == 1) {
		fidbrokenpipe = 0;
		return 1;
	}
	return 0;
}

// Returns 1 if file exists
int tp_exist(const tp_npipe* pipe) {
	struct stat st;
	return stat(pipe->filename, &st);
}

int tp_setsize(const tp_npipe* pipe, const size_t size) {
	//return fcntl(pipe->fid, F_SETPIPE_SZ, (long)size);
	return 0;
}

size_t tp_getsize(const tp_npipe* pipe) {
	//return fcntl(pipe->fid, F_SETPIPE_SZ);
	return 0;
}

#endif
