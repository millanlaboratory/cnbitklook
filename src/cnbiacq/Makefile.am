AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src
lib_LTLIBRARIES = libcnbiacq.la

libcnbiacq_includedir=$(includedir)/cnbiacq

libcnbiacq_include_HEADERS = CaDevice.hpp \
							 CaWriter.hpp

libcnbiacq_la_SOURCES = CaDevice.cpp \
						CaDevice.hpp \
						CaWriter.hpp \
						CaWriter.cpp

libcnbiacq_la_LDFLAGS = $(AM_LDFLAGS) \
						-version-info $(CURRENT):$(REVISION):$(AGE)

libcnbiacq_la_LIBADD =  $(ACQ_LIBS) \
						$(builddir)/../cnbicore/libcnbicore.la\
						$(builddir)/../ndf/libndf.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbiacq.pc
