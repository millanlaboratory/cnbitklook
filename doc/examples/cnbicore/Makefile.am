AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = ccmutex \
				 ccobject \
				 ccsemaphore \
				 ccstreamer \
				 ccthread \
				 ccmultithread \
				 cctime \
				 ccclient \
				 cclogentry \
				 cclogger \
				 ccserver \
				 ccthreadsafe \
				 ccprocess \
				 ccptable \
				 ccpipewriter \
				 ccpipereader \
				 ccpipeserver \
				 ccdoublebuffer \
				 ccendpoint \
				 ccfile \
				 ccpipeasreader \
				 ccpipeaswriter

ccptable_SOURCES = ccptable.cpp
ccprocess_SOURCES = ccprocess.cpp
ccthreadsafe_SOURCES = ccthreadsafe.cpp
ccthread_SOURCES = ccthread.cpp
ccmultithread_SOURCES = ccmultithread.cpp
cctime_SOURCES = cctime.cpp
cclogger_SOURCES = cclogger.cpp
cclogentry_SOURCES = cclogentry.cpp
ccstreamer_SOURCES = ccstreamer.cpp
ccmutex_SOURCES = ccmutex.cpp
ccsemaphore_SOURCES = ccsemaphore.cpp
ccobject_SOURCES = ccobject.cpp
ccclient_SOURCES = ccclient.cpp
ccserver_SOURCES = ccserver.cpp
ccpipeasreader_SOURCES = ccpipeasreader.cpp
ccpipeaswriter_SOURCES = ccpipeaswriter.cpp
ccpipewriter_SOURCES = ccpipewriter.cpp
ccpipereader_SOURCES = ccpipereader.cpp
ccpipeserver_SOURCES = ccpipeserver.cpp
ccdoublebuffer_SOURCES = ccdoublebuffer.cpp
ccendpoint_SOURCES = ccendpoint.cpp
ccfile_SOURCES = ccfile.cpp

LDADD = $(top_builddir)/src/cnbicore/libcnbicore.la
