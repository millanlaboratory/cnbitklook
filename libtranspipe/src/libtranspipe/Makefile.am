# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libtranspipe.la

libtranspipe_includedir=$(includedir)/libtranspipe
libtranspipe_include_HEADERS = tp_namedpipe.h
libtranspipe_la_SOURCES = tp_namedpipe.c \
						  tp_namedpipe.h

libtranspipe_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 