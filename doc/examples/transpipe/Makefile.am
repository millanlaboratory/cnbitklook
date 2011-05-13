AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = tp_namedsender tp_namedreceiver

tp_namedsender_SOURCES = tp_namedsender.c
tp_namedreceiver_SOURCES = tp_namedreceiver.c

LDADD = $(top_builddir)/src/transpipe/libtranspipe.la
