# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libcnbiconfig.la

libcnbiconfig_includedir=$(includedir)/libcnbiconfig
libcnbiconfig_include_HEADERS = CCfgConfig.hpp \
								CCfgTask.hpp \
								CCfgTaskset.hpp \
								CCfgXMLConfig.hpp \
								CCfgTypes.hpp \
								XMLType.hpp \
								XMLDocument.hpp \
								XMLException.hpp

libcnbiconfig_la_SOURCES = CCfgConfig.cpp \
						   CCfgConfig.hpp \
						   CCfgTask.cpp \
						   CCfgTask.hpp \
						   CCfgTaskset.cpp \
						   CCfgTaskset.hpp \
						   CCfgXMLConfig.cpp \
						   CCfgXMLConfig.hpp \
						   CCfgTypes.hpp \
						   XMLTools.hpp \
						   XMLTools.cpp \
						   XMLType.cpp \
						   XMLType.hpp \
						   XMLDocument.cpp \
						   XMLDocument.hpp \
						   XMLException.cpp \
						   XMLException.hpp
						   
libcnbiconfig_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
