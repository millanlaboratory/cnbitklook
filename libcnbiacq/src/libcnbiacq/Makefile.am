# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libcnbiacq.la

libcnbiacq_includedir=$(includedir)/libcnbiacq
libcnbiacq_include_HEADERS = ClDevice.hpp \
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
							  ClLoop.hpp \
							  ClTobiIc.hpp \
							  ClTobiId.hpp

libcnbiacq_la_SOURCES = ClDevice.cpp \
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
						 ClLoop.cpp \
						 ClTobiIc.cpp \
						 ClTobiIc.hpp \
						 ClTobiId.cpp \
						 ClTobiId.hpp

libcnbiacq_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
