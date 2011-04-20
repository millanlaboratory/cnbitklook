AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = resolver \
				  clientchar \
				  serverchar \
				  clientvoid \
				  servervoid \
				  multi 

multi_SOURCES = multi.c
resolver_SOURCES = resolver.c
clientchar_SOURCES = clientchar.c
serverchar_SOURCES = serverchar.c
clientvoid_SOURCES = clientvoid.c
servervoid_SOURCES = servervoid.c

LDADD = $(top_builddir)/src/libtransport/libtransport.la
