# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libcnbiloop

noinst_PROGRAMS = clacqlang \
				  clprolang \
				  clnameslang \
				  clacqclient \
				  clproclient \
				  clproclient2 \
				  clnamesclient \
				  clloop \
				  cltobiic \
				  cltobiid 

clacqlang_SOURCES = clacqlang.cpp
clprolang_SOURCES = clprolang.cpp
clnameslang_SOURCES = clnameslang.cpp
clacqclient_SOURCES = clacqclient.cpp
clproclient_SOURCES = clproclient.cpp
clproclient2_SOURCES = clproclient2.cpp
clnamesclient_SOURCES = clnamesclient.cpp
clloop_SOURCES = clloop.cpp
cltobiic_SOURCES = cltobiic.cpp
cltobiid_SOURCES = cltobiid.cpp

LDADD = $(top_builddir)/src/libcnbiloop/libcnbiloop.la
