AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = clacqlang \
				 clprolang \
				 clnameslang \
				 clacqclient \
				 clproclient \
				 clproclient2 \
				 clnamesclient \
				 clloop \
				 clloopconfig \
				 cltobiic \
				 cltobiic_sender \
				 cltobiic_receiver 

clacqlang_SOURCES = clacqlang.cpp
clprolang_SOURCES = clprolang.cpp
clnameslang_SOURCES = clnameslang.cpp
clacqclient_SOURCES = clacqclient.cpp
clproclient_SOURCES = clproclient.cpp
clproclient2_SOURCES = clproclient2.cpp
clnamesclient_SOURCES = clnamesclient.cpp
clloop_SOURCES = clloop.cpp
clloopconfig_SOURCES = clloopconfig.cpp
cltobiic_SOURCES = cltobiic.cpp
cltobiic_sender_SOURCES = cltobiic_sender.cpp
cltobiic_receiver_SOURCES = cltobiic_receiver.cpp

LDADD = $(top_builddir)/src/cnbiloop/libcnbiloop.la\
		$(top_builddir)/src/cnbicore/libcnbicore.la\
		$(TOBICORE_LIBS) \
		$(TOBIIC_LIBS) \
		$(TOBIID_LIBS)
