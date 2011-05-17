AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

check_PROGRAMS = cnbiconfig_online \
				 cnbiconfig_offline \
				 cnbitask \
				 xmlconfig \
				 smarttype

cnbiconfig_online_SOURCES = cnbiconfig_online.cpp
cnbiconfig_offline_SOURCES = cnbiconfig_offline.cpp
cnbitask_SOURCES = cnbitask.cpp
xmlconfig_SOURCES = xmlconfig.cpp
smarttype_SOURCES = smarttype.cpp 

LDADD = $(top_builddir)/src/cnbiconfig/libcnbiconfig.la
