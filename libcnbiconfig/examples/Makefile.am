# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libcnbiconfig

noinst_PROGRAMS = cnbiconfig \
				  cnbitask \
				  xmlconfig \
				  smarttype

cnbiconfig_SOURCES = cnbiconfig.cpp
cnbitask_SOURCES = cnbitask.cpp
xmlconfig_SOURCES = xmlconfig.cpp
smarttype_SOURCES = smarttype.cpp 

LDADD = $(top_builddir)/src/libcnbiconfig/libcnbiconfig.la
