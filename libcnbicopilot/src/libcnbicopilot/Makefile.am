# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libcnbicopilot.la

libcnbicopilot_includedir = $(includedir)/libcnbicopilot
libcnbicopilot_include_HEADERS = stimula.h \
								generator.h

libcnbicopilot_la_SOURCES = stimula.h\
						   stimula.c\
						   generator.h\
						   generator.c

libcnbicopilot_la_LDFLAGS = $(AM_CFLAGS) \
						   -version-info $(CURRENT):$(REVISION):$(AGE) \
						   -release $(RELEASE) 
