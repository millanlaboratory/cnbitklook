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
AC_SEARCH_LIBS([tr_init_socket], [transport],
        [], AC_MSG_ERROR([The libtransport library must be installed.]))
AC_SEARCH_LIBS([tp_init], [transpipe],
        [], AC_MSG_ERROR([The libtranspipe library must be installed.]))
AC_SEARCH_LIBS([pthread_create], [pthread],
        [], AC_MSG_ERROR([The libpthread library must be installed.]))

AC_HEADER_STDBOOL

AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile doc/Makefile \
	src/libcnbicore/Makefile doc/examples/cnbicore/Makefile src/libcnbicore/cnbicore.pc
	src/libndf/Makefile doc/examples/ndf/Makefile src/libndf/ndf.pc])
AC_OUTPUT
