AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src
lib_LTLIBRARIES = libcnbiconfig.la

bin_PROGRAMS = ccfg_validate ccfg_cli ccfg_getstring ccfg_getint ccfg_getfloat ccfg_getbool

libcnbiconfig_includedir=$(includedir)/cnbiconfig

libcnbiconfig_include_HEADERS = CCfgConfig.hpp \
								CCfgTask.hpp \
								CCfgTaskset.hpp \
								CCfgXMLConfig.hpp \
								CCfgTypes.hpp \
								XMLType.hpp \
								XMLDocument.hpp \
								XMLException.hpp \
								rapidxml.hpp \
								rapidxml_iterators.hpp \
								rapidxml_print.hpp \
								rapidxml_utils.hpp

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
						   
libcnbiconfig_la_LDFLAGS = $(AM_LDFLAGS) -ltobicore \
						   -version-info $(CURRENT):$(REVISION):$(AGE) 

libcnbiconfig_la_LIBADD = $(TOBIIC_LIBS) \
						  $(TOBIID_LIBS)

LDADD = libcnbiconfig.la
ccfg_validate_SOURCES = ccfg_validate.cpp
ccfg_cli_SOURCES = ccfg_cli.cpp
ccfg_getstring_SOURCES = ccfg_getstring.cpp
ccfg_getint_SOURCES = ccfg_getint.cpp
ccfg_getfloat_SOURCES = ccfg_getfloat.cpp
ccfg_getbool_SOURCES = ccfg_getbool.cpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbiconfig.pc
