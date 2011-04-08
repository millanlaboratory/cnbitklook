#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# 09/09/22  Michele Tavella <michele.tavella@epfl.ch>
# If the library source code has changed at all since the last update, then
# increment revision.
# If any interfaces have been added, removed, or changed since the last update,
# increment current, and set revision to 0.
# If any interfaces have been added since the last public release, then
# increment age.  
# If any interfaces have been removed since the last public release, then set
# age to 0.
m4_define([lib_release],0)
m4_define([lib_current],0)
m4_define([lib_revision],2)
m4_define([lib_age],0)

# Setup Automake
AC_PREREQ([2.63])
AC_INIT(libcnbicopilot,[lib_release.lib_current.lib_age.lib_revision],luca.tonin@epfl.ch)
AC_CONFIG_SRCDIR([src/libcnbicopilot/stimula.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config/config.h])

# Setup Doxygen 
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(libcnbicopilot, doc/doxygen.cfg, doc/)

# Initialize Automake
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

##########################################################################
# Windows target support
##########################################################################
case $host in
	*win32* | *mingw32* | *cygwin* | *windows*)
		os_support=win32
		AM_LDFLAGS="$AM_LDFLAGS -no-undefined"
		;;
	*)
		os_support=other
		;;
esac
##########################################################################

AC_SUBST([RELEASE],[lib_release])
AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile \
				 src/libcnbicopilot/Makefile \
				 examples/Makefile \
				 doc/Makefile \
				 cnbicopilot.pc])
AC_OUTPUT
