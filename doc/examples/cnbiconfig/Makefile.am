AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = ccfgconfig_online \
				 ccfgconfig_offline \
				 ccfgtask \
				 ccfgxmlconfig \
				 xmltype

ccfgconfig_online_SOURCES =  ccfgconfig_online.cpp
ccfgconfig_offline_SOURCES = ccfgconfig_offline.cpp
ccfgtask_SOURCES = ccfgtask.cpp
ccfgxmlconfig_SOURCES = ccfgxmlconfig.cpp
xmltype_SOURCES = xmltype.cpp 

LDADD = $(top_builddir)/src/cnbiconfig/libcnbiconfig.la
