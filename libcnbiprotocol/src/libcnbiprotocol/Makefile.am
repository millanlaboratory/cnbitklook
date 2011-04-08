# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libcnbiprotocol.la

libcnbiprotocol_includedir=$(includedir)/libcnbiprotocol
libcnbiprotocol_include_HEADERS = CpAudio.hpp \
								  CpPlayer.hpp \
								  CpRandInt.hpp \
								  CpSound.hpp \
								  CpTrials.hpp \
								  CpTrack.hpp \
								  CpTriggerBeep.hpp \
								  CpTrigger.hpp \
								  CpTriggerLPT.hpp \
								  CpTriggerFactory.hpp \
								  CpProbability.hpp

libcnbiprotocol_la_SOURCES = CpAudio.cpp \
							 CpAudio.hpp \
							 CpPlayer.cpp \
							 CpPlayer.hpp \
							 CpRandInt.cpp \
							 CpRandInt.hpp \
							 CpSound.cpp \
							 CpSound.hpp \
							 CpTrials.cpp \
							 CpTrials.hpp \
							 CpTrack.cpp \
							 CpTrack.hpp \
							 CpTriggerBeep.cpp \
							 CpTriggerBeep.hpp \
							 CpTrigger.cpp \
							 CpTrigger.hpp \
							 CpTriggerLPT.cpp \
							 CpTriggerLPT.hpp \
							 CpTriggerFactory.cpp \
							 CpTriggerFactory.hpp \
							 CpProbability.cpp \
							 CpProbability.hpp

libcnbiprotocol_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
