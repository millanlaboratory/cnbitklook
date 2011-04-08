# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libndf.la

libndf_includedir=$(includedir)/libndf
libndf_include_HEADERS = ndf_codec.h \
						 ndf_presets.h \
						 ndf_types.h

libndf_la_SOURCES = ndf_codec.c \
					ndf_codec.h \
					ndf_presets.c \
					ndf_presets.h \
					ndf_types.c \
					ndf_types.h

libndf_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
