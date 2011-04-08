# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libndf

noinst_PROGRAMS = ndfexample \
				  ndfcopy 

ndfexample_SOURCES = ndfexample.c
ndfcopy_SOURCES = ndfcopy.c

LDADD = $(top_builddir)/src/libndf/libndf.la
