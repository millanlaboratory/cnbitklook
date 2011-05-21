AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = tpsender tpreceiver

tpsender_SOURCES = tpsender.c
tpreceiver_SOURCES = tpreceiver.c

LDADD = $(top_builddir)/src/transpipe/libtranspipe.la
