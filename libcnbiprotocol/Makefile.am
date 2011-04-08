# Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
#AUTOMAKE_OPTIONS = check-news gnu subdir-objects

ACLOCAL_AMFLAGS=-I m4
SUBDIRS = src/libcnbiprotocol \
		  examples
 
include aminclude.am

MOSTLYCLEANFILES = $(DX_CLEANFILES)

EXTRA_DIST = autogen.sh $(DX_CONFIG) doc/html scripts

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbiprotocol.pc
