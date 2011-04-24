# If the library source code has changed at all since the last update, then
# increment revision.
# If any interfaces have been added, removed, or changed since the last update,
# increment current, and set revision to 0.
# If any interfaces have been added since the last public release, then
# increment age.  
# If any interfaces have been removed since the last public release, then set
# age to 0.
m4_define([lib_current],2)
m4_define([lib_revision],0)
m4_define([lib_age],0)

# Setup Autoconf
AC_INIT(cnbiloop,[lib_release.lib_current.lib_age.lib_revision],michele.tavella@epfl.ch)
AC_CONFIG_SRCDIR([src/libndf/ndf_codec.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config/config.h])

# Setup automake
AM_INIT_AUTOMAKE([no-dist-gzip dist-bzip2 color-tests subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX

LT_INIT([win32-dll disable-static])
AC_SUBST(LIBTOOL_DEPS)

# Checks for libraries.
save_LIBS="$LIBS"
AC_SEARCH_LIBS([pthread_create], [pthread],
        AC_SUBST([PTHREAD_LIBS], "$LIBS"), AC_MSG_ERROR([The libpthread library must be installed.]))
LIBS="$save_LIBS"

# Check acquisition libs
save_LIBS="$LIBS"
AC_SEARCH_LIBS([xdf_open], [xdffileio], [],
        AC_MSG_ERROR([The xdffileio library must be installed.]))
AC_SEARCH_LIBS([egd_open], [eegdev], [],
        AC_MSG_ERROR([The eegdev library must be installed.]))
AC_SUBST([ACQ_LIBS], "$LIBS")
LIBS="$save_LIBS"


AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([rapidxml.hpp rapidxml_print.hpp], [], 
       AC_MSG_ERROR([The RapidXML headers must be installed.]), [])
AC_LANG_POP([C++])

save_LIBS="$LIBS"
AC_SEARCH_LIBS_CLASS([ICMessage], [tobiic], [#include <libtobiic/ICMessage.hpp>],
            AC_SUBST([TOBIIC_LIBS], "$LIBS"), AC_MSG_ERROR([The tobiic library cannot be found.]))
LIBS="$save_LIBS"

save_LIBS="$LIBS"
AC_SEARCH_LIBS_CLASS([IDMessage], [tobiid], [#include <libtobiid/IDMessage.hpp>],
            AC_SUBST([TOBIID_LIBS], "$LIBS"), AC_MSG_ERROR([The tobiid library cannot be found.]))
LIBS="$save_LIBS"

AC_HEADER_STDBOOL

AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile doc/Makefile extra/Makefile doc/doxygen.cfg
	src/libcnbicore/Makefile doc/examples/cnbicore/Makefile src/libcnbicore/cnbicore.pc
	src/libtransport/Makefile doc/examples/transport/Makefile src/libtransport/transport.pc
	src/libtranspipe/Makefile doc/examples/transpipe/Makefile src/libtranspipe/transpipe.pc
	src/libcnbiacq/Makefile src/libcnbiacq/cnbiacq.pc
	src/libcnbiconfig/Makefile doc/examples/cnbiconfig/Makefile src/libcnbiconfig/cnbiconfig.pc
	src/libcnbiloop/Makefile doc/examples/cnbiloop/Makefile src/libcnbiloop/cnbiloop.pc
	src/libndf/Makefile doc/examples/ndf/Makefile src/libndf/ndf.pc])
AC_OUTPUT
