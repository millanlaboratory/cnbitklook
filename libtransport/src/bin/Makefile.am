# 2009-11-05  Michele Tavella <michele.tavella@epfl.ch>
AM_CFLAGS = -I$(top_srcdir)/src/libtransport 

bin_PROGRAMS = tr_tcpserver\
			   tr_udpserver\
			   tr_tcpmonitor\
			   tr_udpmonitor\
			   tr_tcpclient\
			   tr_udpclient

tr_udpserver_SOURCES = tr_udpserver.c
tr_tcpserver_SOURCES = tr_tcpserver.c
tr_tcpclient_SOURCES = tr_tcpclient.c
tr_udpclient_SOURCES = tr_udpclient.c
tr_udpmonitor_SOURCES = tr_udpmonitor.c
tr_tcpmonitor_SOURCES = tr_tcpmonitor.c

LDADD = $(top_builddir)/src/libtransport/libtransport.la
