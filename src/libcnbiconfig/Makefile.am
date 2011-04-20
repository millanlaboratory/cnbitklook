lib_LTLIBRARIES = libcnbiconfig.la
bin_PROGRAMS = ccfg_validate ccfg_cli

libcnbiconfig_includedir=$(includedir)/libcnbiconfig
libcnbiconfig_include_HEADERS = CCfgConfig.hpp \
				CCfgTask.hpp \
				CCfgTaskset.hpp \
				CCfgXMLConfig.hpp \
				CCfgTypes.hpp \
				XMLType.hpp \
				XMLDocument.hpp \
				XMLException.hpp

libcnbiconfig_la_SOURCES = CCfgConfig.cpp CCfgConfig.hpp \
			   CCfgTask.cpp CCfgTask.hpp \
			   CCfgTaskset.cpp CCfgTaskset.hpp \
			   CCfgXMLConfig.cpp CCfgXMLConfig.hpp \
			   CCfgTypes.hpp XMLTools.hpp \
			   XMLTools.cpp XMLType.cpp \
			   XMLType.hpp \
			   XMLDocument.cpp XMLDocument.hpp \
			   XMLException.cpp XMLException.hpp
						   
libcnbiconfig_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE) 
libcnbiconfig_la_LIBADD = $(TOBIIC_LIBS)


ccfg_validate_SOURCES = ccfg_validate.cpp
ccfg_validate_LDADD = libcnbiconfig.la

ccfg_cli_SOURCES = ccfg_cli.cpp
ccfg_cli_LDADD = libcnbiconfig.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbiconfig.pc
