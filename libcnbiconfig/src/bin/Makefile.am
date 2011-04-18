# 2009-11-05  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libcnbiconfig 

bin_PROGRAMS = ccfg_validate \
			   ccfg_cli

ccfg_validate_SOURCES = ccfg_validate.cpp
ccfg_cli_SOURCES = ccfg_cli.cpp

LDADD = $(top_builddir)/src/libcnbiconfig/libcnbiconfig.la
