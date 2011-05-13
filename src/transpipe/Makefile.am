lib_LTLIBRARIES = libtranspipe.la

libtranspipe_includedir=$(includedir)/transpipe
libtranspipe_include_HEADERS = tp_namedpipe.h
libtranspipe_la_SOURCES = tp_namedpipe.c tp_namedpipe.h
libtranspipe_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = transpipe.pc
