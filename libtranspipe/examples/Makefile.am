# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libtranspipe

noinst_PROGRAMS = tp_namedsender \
				  tp_namedreceiver

tp_namedsender_SOURCES = tp_namedsender.c
tp_namedreceiver_SOURCES = tp_namedreceiver.c

LDADD = $(top_builddir)/src/libtranspipe/libtranspipe.la
