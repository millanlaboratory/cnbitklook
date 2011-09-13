# 2011-07-08  Michele Tavella <michele.tavella@epfl.ch>
# - If the library source code has changed at all since the last update,
#   then increment revision.
# - If any interfaces have been added, removed, or changed since the last
#   update, increment current, and set revision to 0.
# - If any interfaces have been added since the last public release, then
#   increment age.
# - If any interfaces have been removed since the last public release, then
#   set age to 0.

m4_define([lib_current],5)
m4_define([lib_revision],1)
m4_define([lib_age],1)

AC_INIT(cnbiloop,[lib_current.lib_revision.lib_age],
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


#AC_LANG_PUSH([C++])
#AC_CHECK_HEADERS([rapidxml.hpp rapidxml_print.hpp], [], 
#       AC_MSG_ERROR([The RapidXML headers must be installed.]), [])
#AC_LANG_POP([C++])

save_LIBS="$LIBS"
AC_SEARCH_LIBS_CLASS([TCBlock], [tobicore], [#include <tobicore/TCBlock.hpp>],
            AC_SUBST([TOBICORE_LIBS], "$LIBS"), 
			AC_MSG_ERROR([The tobicore library cannot be found.]))
LIBS="$save_LIBS"

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

AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen] [no])
AC_CHECK_PROG([MWRAP], [mwrap], [mwrap], [no])
AC_CHECK_PROG([MEX], [mex], [mex], [no])

if test "$MWRAP" = no ; then 
	AC_MSG_ERROR(Cannot find mwrap.) 
fi
if test "$MEX" = no ; then 
	AC_MSG_ERROR(Cannot find mex.) 
fi
if test "$DOXYGEN" = no ; then 
	AC_MSG_ERROR(Cannot find doxygen.) 
fi

AC_HEADER_STDBOOL

#AX_DEFINE_DIR([SYSCONFDIR], [sysconfdir], [System configuration path])

AC_SUBST([RELEASE],[lib_release])
AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile\
				 doc/Makefile\
				 doc/doxygen.cfg\
				 doc/examples/cnbiconfig/Makefile\
				 doc/examples/cnbicore/Makefile\
				 doc/examples/cnbiloop/Makefile\
				 doc/examples/ndf/Makefile\
				 doc/examples/transpipe/Makefile\
				 doc/examples/transport/Makefile\
				 extra/Makefile\
				 mex/Makefile\
				 mex/cnbitkmat/Makefile\
				 mex/matndf/Makefile\
				 mex/mexcnbiconfig/Makefile\
				 mex/mexcnbiloop/Makefile\
				 mex/mextransport/Makefile\
				 mex/mtpath/Makefile\
				 src/cnbiacq/Makefile\
				 src/cnbiacq/cnbiacq.pc\
				 src/cnbiconfig/Makefile\
				 src/cnbiconfig/cnbiconfig.pc\
				 src/cnbicore/Makefile\
				 src/cnbicore/cnbicore.pc\
				 src/cnbiloop/Makefile\
				 src/cnbiloop/cnbiloop.pc\
				 src/ndf/Makefile\
				 src/ndf/ndf.pc\
				 src/transpipe/Makefile\
				 src/transpipe/transpipe.pc\
				 src/transport/Makefile\
				 src/transport/transport.pc])

AC_OUTPUT
