# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libcnbiloop.la

libcnbiloop_includedir=$(includedir)/libcnbiloop
libcnbiloop_include_HEADERS = ClDevice.hpp \
							  ClMatlab.hpp \
							  ClLanguage.hpp \
							  ClAcqLang.hpp \
							  ClProLang.hpp \
							  ClNamesLang.hpp \
							  ClWriter.hpp \
							  ClAcqAsServer.hpp \
							  ClProAsServer.hpp \
							  ClNamesAsServer.hpp \
							  ClAcqClient.hpp \
							  ClNamesClient.hpp \
							  ClProClient.hpp \
							  ClClient.hpp \
							  ClLoop.hpp

libcnbiloop_la_SOURCES = ClDevice.cpp \
						 ClDevice.hpp \
						 ClMatlab.cpp \
						 ClMatlab.hpp \
						 ClLanguage.cpp \
						 ClLanguage.hpp \
						 ClAcqLang.cpp \
						 ClAcqLang.hpp \
						 ClProLang.cpp \
						 ClProLang.hpp \
						 ClNamesLang.cpp \
						 ClNamesLang.hpp \
						 ClWriter.hpp \
						 ClWriter.cpp \
						 ClAcqAsServer.hpp \
						 ClAcqAsServer.cpp \
						 ClProAsServer.hpp \
						 ClProAsServer.cpp \
						 ClNamesAsServer.hpp \
						 ClNamesAsServer.cpp \
						 ClNamesClient.hpp \
						 ClNamesClient.cpp \
						 ClAcqClient.hpp \
						 ClAcqClient.cpp \
						 ClProClient.hpp \
						 ClProClient.cpp \
						 ClClient.hpp \
						 ClClient.cpp \
						 ClLoop.hpp \
						 ClLoop.cpp

libcnbiloop_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
