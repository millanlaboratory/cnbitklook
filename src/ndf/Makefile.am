lib_LTLIBRARIES = libndf.la

libndf_includedir=$(includedir)/ndf
libndf_include_HEADERS = ndf_codec.h ndf_presets.h ndf_types.h

libndf_la_SOURCES = ndf_codec.c	ndf_codec.h \
		    ndf_presets.c ndf_presets.h \
		    ndf_types.c ndf_types.h

libndf_la_LDFLAGS = -version-info $(CURRENT):$(REVISION):$(AGE) 

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ndf.pc
