lib_LTLIBRARIES = libtransport.la
bin_PROGRAMS = tr_tcpserver\
			   tr_udpserver\
			   tr_tcpmonitor\
			   tr_udpmonitor\
			   tr_tcpclient\
			   tr_udpclient

libtransport_includedir=$(includedir)/transport
libtransport_include_HEADERS = tr_net.h\
							   tr_tcp.h\
							   tr_udp.h\
							   tr_names.h\
							   tr_types.h

libtransport_la_SOURCES = tr_net.h\
						  tr_net.c\
						  tr_tcp.h\
						  tr_tcp.c\
						  tr_udp.h\
						  tr_udp.c\
						  tr_names.h\
						  tr_names.c\
						  tr_types.h

libtransport_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE)
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = transport.pc


tr_udpserver_SOURCES = tr_udpserver.c
tr_udpserver_LDADD = libtransport.la
tr_tcpserver_SOURCES = tr_tcpserver.c
tr_tcpserver_LDADD = libtransport.la
tr_tcpclient_SOURCES = tr_tcpclient.c
tr_tcpclient_LDADD = libtransport.la
tr_udpclient_SOURCES = tr_udpclient.c
tr_udpclient_LDADD = libtransport.la
tr_udpmonitor_SOURCES = tr_udpmonitor.c
tr_udpmonitor_LDADD = libtransport.la
tr_tcpmonitor_SOURCES = tr_tcpmonitor.c
tr_tcpmonitor_LDADD = libtransport.la
