lib_LTLIBRARIES = libcnbicore.la
AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src

libcnbicore_includedir = $(includedir)/cnbicore
libcnbicore_include_HEADERS = CcBasic.hpp \
			  CcCallback.hpp \
			  CcClient.hpp \
			  CcCore.hpp \
			  CcDeserializer.hpp \
			  CcEndpoint.hpp \
			  CcException.hpp \
			  CcFlags.hpp \
			  CcLog.hpp \
			  CcLogEntry.hpp \
			  CcLogger.hpp \
			  CcMutex.hpp \
			  CcNetworkTypes.hpp \
			  CcObject.hpp \
			  CcPipe.hpp \
			  CcPipeServer.hpp\
			  CcPipeSink.hpp \
			  CcPipeSource.hpp \
			  CcPipeWriter.hpp \
			  CcPipeReader.hpp \
			  CcProcess.hpp \
			  CcProxy.hpp \
			  CcPtable.hpp \
			  CcSemaphore.hpp \
			  CcServer.hpp\
			  CcServerEndpoint.hpp \
			  CcServerMulti.hpp \
			  CcServerSingle.hpp \
			  CcSocket.hpp \
			  CcStreamer.hpp \
			  CcThread.hpp \
			  CcThreadSafe.hpp \
			  CcThreadSafe.cpp \
			  CcThreadrunner.hpp \
			  CcTime.hpp \
			  CcDoubleBuffer.hpp \
			  CcBuffer.cpp \
			  CcBuffer.hpp \
			  CcFile.cpp \
			  CcFile.hpp

libcnbicore_la_SOURCES = CcBasic.hpp \
			CcPipeServer.cpp \
			CcPipeServer.hpp \
			CcCallback.hpp \
			CcClient.cpp \
			CcClient.hpp \
			CcCore.cpp \
			CcCore.hpp \
			CcDeserializer.hpp \
			CcEndpoint.cpp \
			CcEndpoint.hpp \
			CcException.cpp \
			CcException.hpp \
			CcFlags.hpp \
			CcLog.hpp \
			CcLogEntry.cpp \
			CcLogEntry.hpp \
			CcLogger.cpp \
			CcLogger.hpp \
			CcMutex.cpp \
			CcMutex.hpp \
			CcNetworkTypes.cpp \
			CcNetworkTypes.hpp \
			CcObject.cpp \
			CcObject.hpp \
			CcPipe.cpp \
			CcPipe.hpp \
			CcPipeReader.cpp \
			CcPipeReader.hpp \
			CcPipeSink.cpp \
			CcPipeSink.hpp \
			CcPipeSource.cpp \
			CcPipeSource.hpp \
			CcPipeWriter.cpp \
			CcPipeWriter.hpp \
			CcProcess.cpp \
			CcProcess.hpp \
			CcProxy.hpp \
			CcPtable.cpp \
			CcPtable.hpp \
			CcSemaphore.cpp \
			CcSemaphore.hpp \
			CcServer.cpp \
			CcServer.hpp \
			CcServerEndpoint.cpp \
			CcServerEndpoint.hpp \
			CcServerMulti.cpp \
			CcServerMulti.hpp \
			CcServerSingle.cpp \
			CcServerSingle.hpp \
			CcSocket.cpp \
			CcSocket.hpp \
			CcStreamer.cpp \
			CcStreamer.hpp \
			CcThread.cpp \
			CcThread.hpp \
			CcThreadSafe.cpp \
			CcThreadSafe.hpp \
			CcThreadrunner.cpp \
			CcThreadrunner.hpp \
			CcTime.cpp \
			CcTime.hpp \
			CcDoubleBuffer.cpp \
			CcDoubleBuffer.hpp \
			CcBuffer.cpp \
			CcBuffer.hpp \
			CcFile.cpp \
			CcFile.hpp

libcnbicore_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE)
libcnbicore_la_LIBADD = $(top_builddir)/src/transport/libtransport.la \
                        $(top_builddir)/src/transpipe/libtranspipe.la \
			$(PTHREAD_LIBS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cnbicore.pc
