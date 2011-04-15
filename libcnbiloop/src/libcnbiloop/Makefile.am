# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libcnbiloop.la

libcnbiloop_includedir=$(includedir)/libcnbiloop
libcnbiloop_include_HEADERS = ClMatlab.hpp \
							  ClLanguage.hpp \
							  ClAcqLang.hpp \
							  ClProLang.hpp \
							  ClNamesLang.hpp \
							  ClAcqClient.hpp \
							  ClNamesClient.hpp \
							  ClProClient.hpp \
							  ClClient.hpp \
							  ClLoop.hpp \
							  ClTobiIc.hpp \
							  ClTobiId.hpp

libcnbiloop_la_SOURCES = ClMatlab.cpp \
						 ClMatlab.hpp \
						 ClLanguage.cpp \
						 ClLanguage.hpp \
						 ClAcqLang.cpp \
						 ClAcqLang.hpp \
						 ClProLang.cpp \
						 ClProLang.hpp \
						 ClNamesLang.cpp \
						 ClNamesLang.hpp \
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

libcnbiloop_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
