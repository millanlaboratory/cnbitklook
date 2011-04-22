lib_LTLIBRARIES = libcnbiloop.la

bin_PROGRAMS = cl_processing \
	cl_monitor \
	cl_nameserver \
	cl_tidmonitor \
	cl_ticmonitor \
	cl_checkloop \
	cl_rpc \
	cl_acquisition 

dist_bin_SCRIPTS = cl_rpcnotify \
	           cl_grpc \
		   cl_screenloop \
		   cl_attachloop

AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

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

libcnbiloop_la_SOURCES = ClMatlab.cpp ClMatlab.hpp \
			 ClLanguage.cpp ClLanguage.hpp \
			 ClAcqLang.cpp ClAcqLang.hpp \
			 ClProLang.cpp ClProLang.hpp \
			 ClNamesLang.cpp ClNamesLang.hpp \
			 ClNamesClient.hpp ClNamesClient.cpp \
			 ClAcqClient.hpp ClAcqClient.cpp \
			 ClProClient.hpp ClProClient.cpp \
			 ClClient.hpp ClClient.cpp \
			 ClLoop.hpp ClLoop.cpp \
			 ClTobiIc.cpp ClTobiIc.hpp \
			 ClTobiId.cpp ClTobiId.hpp

libcnbiloop_la_LDFLAGS = $(AM_LDLAGS) -version-info $(CURRENT):$(REVISION):$(AGE) 
libcnbiloop_la_LIBADD = $(builddir)/../libcnbicore/libcnbicore.la \
                        $(builddir)/../libndf/libndf.la \
			$(TOBIIC_LIBS) $(TOBIID_LIBS)

LDADD = libcnbiloop.la

cl_acquisition_SOURCES = cl_acquisition.cpp ClAcqAsServer.cpp ClAcqAsServer.hpp
cl_acquisition_LDADD = libcnbiloop.la $(top_builddir)/src/libcnbiacq/libcnbiacq.la
cl_processing_SOURCES = cl_processing.cpp ClProAsServer.cpp ClProAsServer.hpp
cl_nameserver_SOURCES = cl_nameserver.cpp ClNamesAsServer.cpp ClNamesAsServer.hpp
cl_monitor_SOURCES = cl_monitor.cpp
cl_tidmonitor_SOURCES = cl_tidmonitor.cpp
cl_ticmonitor_SOURCES = cl_ticmonitor.cpp
cl_checkloop_SOURCES = cl_checkloop.cpp
cl_rpc_SOURCES = cl_rpc.cpp

