AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = trresolver \
				 trclientchar \
				 trserverchar \
				 trclientvoid \
				 trservervoid \
				 trmulti 

trmulti_SOURCES = trmulti.c
trresolver_SOURCES = trresolver.c
trclientchar_SOURCES = trclientchar.c
trserverchar_SOURCES = trserverchar.c
trclientvoid_SOURCES = trclientvoid.c
trservervoid_SOURCES = trservervoid.c

LDADD = $(top_builddir)/src/transport/libtransport.la
