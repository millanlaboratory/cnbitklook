m4_define([lib_release],0)
m4_define([lib_current],2)
m4_define([lib_revision],0)
m4_define([lib_age],0)

AC_INIT(cnbiloop,[lib_release.lib_current.lib_age.lib_revision],
		michele.tavella@epfl.ch)
AC_CONFIG_SRCDIR([src/ndf/ndf_codec.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config/config.h])

AM_INIT_AUTOMAKE([no-dist-gzip dist-bzip2 color-tests subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AC_PROG_LIBTOOL
AC_PROG_CXX

LT_INIT([win32-dll disable-static])
AC_SUBST(LIBTOOL_DEPS)

save_LIBS="$LIBS"
AC_SEARCH_LIBS([pthread_create], [pthread],
        AC_SUBST([PTHREAD_LIBS], "$LIBS"), 
		AC_MSG_ERROR([The pthread library must be installed.]))
LIBS="$save_LIBS"

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
AC_SEARCH_LIBS_CLASS([ICMessage], [tobiic], [#include <tobiic/ICMessage.hpp>],
            AC_SUBST([TOBIIC_LIBS], "$LIBS"), 
			AC_MSG_ERROR([The tobiic library cannot be found.]))
LIBS="$save_LIBS"

save_LIBS="$LIBS"
AC_SEARCH_LIBS_CLASS([IDMessage], [tobiid], [#include <tobiid/IDMessage.hpp>],
            AC_SUBST([TOBIID_LIBS], "$LIBS"), 
			AC_MSG_ERROR([The tobiid library cannot be found.]))
LIBS="$save_LIBS"

AC_HEADER_STDBOOL

AC_SUBST([RELEASE],[lib_release])
AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile\
				 doc/Makefile\
				 extra/Makefile\
				 doc/doxygen.cfg\
				 src/cnbicore/Makefile\
				 doc/examples/cnbicore/Makefile\
				 src/cnbicore/cnbicore.pc\
				 src/transport/Makefile\
				 doc/examples/transport/Makefile\
				 src/transport/transport.pc\
				 src/transpipe/Makefile\
				 doc/examples/transpipe/Makefile\
				 src/transpipe/transpipe.pc\
				 src/cnbiacq/Makefile\
				 src/cnbiacq/cnbiacq.pc\
				 src/cnbiconfig/Makefile\
				 doc/examples/cnbiconfig/Makefile\
				 src/cnbiconfig/cnbiconfig.pc\
				 src/cnbiloop/Makefile\
				 doc/examples/cnbiloop/Makefile\
				 src/cnbiloop/cnbiloop.pc\
				 src/ndf/Makefile\
				 doc/examples/ndf/Makefile\
				 src/ndf/ndf.pc\
				 mex/Makefile\
				 mex/mexcnbiconfig/Makefile\
				 mex/mexcnbiconfig/cnbiconfig.mw\
				 mex/mexcnbiloop/Makefile\
				 mex/mexcnbiloop/cnbiloop.mw\
				 mex/mextransport/Makefile\
				 mex/mextransport/transport.mw])

AC_OUTPUT
