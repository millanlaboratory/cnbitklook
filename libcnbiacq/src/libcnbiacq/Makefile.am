# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libcnbiacq.la

libcnbiacq_includedir=$(includedir)/libcnbiacq
libcnbiacq_include_HEADERS = CaDevice.hpp \
							 CaWriter.hpp

libcnbiacq_la_SOURCES = CaDevice.cpp \
						CaDevice.hpp \
						CaWriter.hpp \
						CaWriter.cpp

libcnbiacq_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
