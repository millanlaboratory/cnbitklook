# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CFLAGS = -I$(top_srcdir)/src/libcnbicopilot

noinst_PROGRAMS = stimula\
				  fakefeedback

stimula_SOURCES = stimula.c
fakefeedback_SOURCES = fakefeedback.c

LDADD = $(top_builddir)/src/libcnbicopilot/libcnbicopilot.la
