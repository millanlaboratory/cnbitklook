# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libxmlconfig.la

libxmlconfig_includedir=$(includedir)/libxmlconfig
libxmlconfig_include_HEADERS = Tools.hpp \
							   SmartType.hpp \
							   XMLDocument.hpp \
							   XMLException.hpp

libxmlconfig_la_SOURCES = Tools.hpp \
						  Tools.cpp \
						  SmartType.cpp \
						  SmartType.hpp \
						  XMLDocument.cpp \
						  XMLDocument.hpp \
						  XMLException.cpp \
						  XMLException.hpp

libxmlconfig_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
#libxmlconfig_la_LIBADD = -lhybrid
