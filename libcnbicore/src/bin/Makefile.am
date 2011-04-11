# 2009-11-05  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libcnbicore 

bin_PROGRAMS = cc_endianness \
			   cc_architecture

cc_endianness_SOURCES = cc_endianness.cpp
cc_architecture_SOURCES = cc_architecture.cpp

LDADD = $(top_builddir)/src/libcnbicore/libcnbicore.la
