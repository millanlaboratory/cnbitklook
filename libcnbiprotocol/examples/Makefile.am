# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libcnbiprotocol

noinst_PROGRAMS = cpaudio \
				  cpplayer \
				  cpsound \
				  cptrials \
				  cptrack \
				  cptriggerbeep \
				  cptriggerlpt \
				  cptriggerfactory \
				  cpprobability

cpaudio_SOURCES = cpaudio.cpp
cpplayer_SOURCES = cpplayer.cpp
cpsound_SOURCES = cpsound.cpp
cptrials_SOURCES = cptrials.cpp
cptrack_SOURCES = cptrack.cpp
cptriggerbeep_SOURCES = cptriggerbeep.cpp
cptriggerlpt_SOURCES = cptriggerlpt.cpp
cptriggerfactory_SOURCES = cptriggerfactory.cpp
cpprobability_SOURCES = cpprobability.cpp

LDADD = $(top_builddir)/src/libcnbiprotocol/libcnbiprotocol.la
