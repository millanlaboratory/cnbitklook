AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = clacqlang \
				 clprolang \
				 clnameslang \
				 clacqclient \
				 clproclient \
				 clproclient2 \
				 clnamesclient \
				 clloop \
				 cltobiic

clacqlang_SOURCES = clacqlang.cpp
clprolang_SOURCES = clprolang.cpp
clnameslang_SOURCES = clnameslang.cpp
clacqclient_SOURCES = clacqclient.cpp
clproclient_SOURCES = clproclient.cpp
clproclient2_SOURCES = clproclient2.cpp
clnamesclient_SOURCES = clnamesclient.cpp
clloop_SOURCES = clloop.cpp
cltobiic_SOURCES = cltobiic.cpp

LDADD = $(top_builddir)/src/cnbiloop/libcnbiloop.la\
		$(top_builddir)/src/cnbicore/libcnbicore.la\
		$(TOBICORE_LIBS) \
		$(TOBIIC_LIBS) \
		$(TOBIID_LIBS)
