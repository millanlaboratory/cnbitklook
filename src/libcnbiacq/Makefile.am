lib_LTLIBRARIES = libcnbiacq.la
AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

libcnbiacq_includedir=$(includedir)/libcnbiacq
libcnbiacq_include_HEADERS = CaDevice.hpp CaWriter.hpp

libcnbiacq_la_SOURCES = CaDevice.cpp CaDevice.hpp \
			CaWriter.hpp CaWriter.cpp

libcnbiacq_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE)
libcnbiacq_la_LIBADD =  $(ACQ_LIBS) \
		     $(builddir)/../libcnbicore/libcnbicore.la\
		     $(builddir)/../libndf/libndf.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbiacq.pc
