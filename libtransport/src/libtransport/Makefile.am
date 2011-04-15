# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libtransport.la

libtransport_includedir=$(includedir)/libtransport
libtransport_include_HEADERS = net.h\
							   tcp.h\
							   udp.h\
							   names.h\
							   types.h

libtransport_la_SOURCES = net.h\
						  net.c\
						  tcp.h\
						  tcp.c\
						  udp.h\
						  udp.c\
						  names.h\
						  names.c\
						  types.h

libtransport_la_LDFLAGS = $(AM_CFLAGS) \
						  -version-info $(CURRENT):$(REVISION):$(AGE) \
						  -release $(RELEASE) 
libtransport_la_LIBADD = -lm
