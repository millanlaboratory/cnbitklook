# 2009-11-05  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libcnbiloop 
dist_bin_SCRIPTS = cl_rpcnotify \
				   cl_grpc \
				   cl_screenloop \
				   cl_attachloop

bin_PROGRAMS = cl_acquisition \
			   cl_processing \
			   cl_monitor \
			   cl_nameserver \
			   cl_tidmonitor \
			   cl_ticmonitor \
			   cl_checkloop \
			   cl_rpc

cl_acquisition_SOURCES = cl_acquisition.cpp
cl_nameserver_SOURCES = cl_nameserver.cpp
cl_processing_SOURCES = cl_processing.cpp
cl_monitor_SOURCES = cl_monitor.cpp
cl_tidmonitor_SOURCES = cl_tidmonitor.cpp
cl_ticmonitor_SOURCES = cl_ticmonitor.cpp
cl_checkloop_SOURCES = cl_checkloop.cpp
cl_rpc_SOURCES = cl_rpc.cpp

LDADD = $(top_builddir)/src/libcnbiloop/libcnbiloop.la
