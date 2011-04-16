# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libcnbiconfig

noinst_PROGRAMS = cnbiconfig_online \
				  cnbiconfig_offline \
				  cnbitask \
				  xmlconfig \
				  smarttype

cnbiconfig_online_SOURCES = cnbiconfig_online.cpp
cnbiconfig_offline_SOURCES = cnbiconfig_offline.cpp
cnbitask_SOURCES = cnbitask.cpp
xmlconfig_SOURCES = xmlconfig.cpp
smarttype_SOURCES = smarttype.cpp 

LDADD = $(top_builddir)/src/libcnbiconfig/libcnbiconfig.la
