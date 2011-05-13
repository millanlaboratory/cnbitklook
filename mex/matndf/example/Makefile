# Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
# This file is part of the CNBI Toolkit

PROJECT=ndfmat_examples
VERSION=N/A
AUTHOR=Michele Tavella <tavella.michele@gmail.com>

CC=gcc
CFLAGS=-Wall -O2
LDFLAGS=-lm -lpthread -lstdc++
CTAGS=ctags -R --c++-kinds=+p --fields=+iaS --extra=+q

CNBITK_ROOT=~/Paths/cnbitk/
CONFIG_LIBS=ndf transport transpipe
CONFIG_CFLAGS=$(shell pkg-config --cflags $(CONFIG_LIBS))
CONFIG_LDFLAGS=$(shell pkg-config --libs $(CONFIG_LIBS))

OBJ_SRC=$(wildcard *.c)
OBJ_BIN=$(OBJ_SRC:.c=)

all: $(OBJ_BIN)
	
%:%.c
	@echo "[Makefile] Building executables"
	$(CC) $(CFLAGS) $(CONFIG_CFLAGS) $(CONFIG_LDFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: tags install clean info edit

clean:
	@echo "[Makefile] Cleaning..."
	@rm -fv $(OBJ_BIN)
	@rm -fv core*
	@rm -fv vgcore*
	@rm -fv tags

install: all
	@echo "[Makefile] Running installation"
	@cp -v $(OBJ_BIN) $(CNBITK_ROOT)/bin

tags:
	@echo "[Makefile] Building tags"
	@$(CTAGS) $(CNBITK_ROOT)/include/ .

info:
	@echo "[Makefile] Info:"
	@echo "  Project:   $(PROJECT)"
	@echo "  Version:   $(VERSION)" 
	@echo "  Author:    $(AUTHOR)"
	@echo "  Binaries:  $(OBJ_BIN)"
	@echo "  Libraries: $(CONFIG_LIBS)"
	@echo "  Local:     $(CNBITK_ROOT)"

edit:
	@echo "[Makefile] Editing:"
	@gvim -geom 175x60 -O2 $(OBJ_SRC) 
