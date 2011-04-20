lib_LTLIBRARIES = libtransport.la
bin_PROGRAMS = tr_tcpserver\
			   tr_udpserver\
			   tr_tcpmonitor\
			   tr_udpmonitor\
			   tr_tcpclient\
			   tr_udpclient

libtransport_includedir=$(includedir)/libtransport
libtransport_include_HEADERS = net.h\
			tcp.h\
			udp.h\
			names.h\
			types.h

libtransport_la_SOURCES = net.h net.c\
			  tcp.h tcp.c\
			  udp.h udp.c\
			  names.h names.c\
			  types.h

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

