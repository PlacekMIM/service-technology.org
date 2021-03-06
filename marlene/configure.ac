AC_INIT([Marlene],[1.3-unreleased],[marlene@service-technology.org],[marlene],[http://service-technology.org/tools/marlene])

AC_MSG_NOTICE([ __  __            _                 ])
AC_MSG_NOTICE([|  \/  | __ _ _ __| | ___ _ __   ___ ])
AC_MSG_NOTICE([| |\/| |/ _` | '__| |/ _ \\ '_ \\ / _ \\])
AC_MSG_NOTICE([| |  | | (_| | |  | |  __/ | | |  __/])
AC_MSG_NOTICE([|_|  |_|\\__,_|_|  |_|\\___|_| |_|\\___|])
AC_MSG_NOTICE([Making adapters behave])
AC_MSG_NOTICE([${PACKAGE_URL}])
AC_MSG_NOTICE([version ${PACKAGE_VERSION} ])
AC_MSG_NOTICE()

AC_CONFIG_SRCDIR([src/marlene.cc])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([build-aux])

# use the service-technology.org macros
AC_ST_BASIC
# AC_ST_WENDY([3.3])
# AC_ST_CANDY([1.0])
AC_ST_AUTOTEST
AC_ST_PNAPI([4.02])
AC_ST_CONFIGPP([1.4])
AC_ST_VERBOSE

# search for optional tools needed by "make check"
AC_PATH_PROG(VALGRIND, [valgrind], [])
AC_PATH_PROG(DIFF, [diff], [])

AC_PATH_PROGS(WENDY, [wendy], [])
AC_PATH_PROGS(CANDY, [candy], [])

CONVERTER=statemachine

AC_PATH_PROGS(GENET, [genet], [])
if test -z "$GENET" ; then
  AC_MSG_WARN([GENET not found. See file README.])
else
  CONVERTER=genet
  DEFCONVERTER=genet
fi

AC_PATH_PROGS(PETRIFY, [petrify4.2 petrify4.1 petrify4.0 petrify], [])
if test -z "$PETRIFY" ; then
  AC_MSG_WARN([Petrify was not found. See file README.])
else 
  CONVERTER=petrify
  if test -z "$GENET" ; then
    DEFCONVERTER=petrify
  fi
fi

AC_MSG_CHECKING([default sa converter])
AC_MSG_RESULT($DEFCONVERTER)
AC_MSG_CHECKING([default sa converter for test cases])
AC_MSG_RESULT($CONVERTER)

AC_SUBST(CONVERTER)
AC_SUBST(DEFCONVERTER)

# enable Windows temp files
AC_ARG_ENABLE(windowstemp,
              AS_HELP_STRING([--enable-windowstemp],[sets standard temp file for Marlene to %TEMP%/marlene-XXXXXX (should only set when compiling for pure Win32 binary)]),
              enable_windowstemp=${enableval}, enable_windowstemp="no")
AC_MSG_CHECKING([whether to use Windows temp folder])
AC_MSG_RESULT($enable_windowstemp)
if test "$enable_windowstemp" = "yes" ; then
    TEMPFOLDER=`cygpath --windows $TEMP`
else
    TEMPFOLDER=/tmp/marlene-XXXXXX
fi

AC_SUBST(TEMPFOLDER)

AC_MSG_CHECKING([weather to be sunny])
AC_MSG_RESULT([better should be])

# Checks for header files.
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([tr1/memory])
AH_BOTTOM([#ifdef __cplusplus
#if defined(HAVE_TR1_MEMORY) and HAVE_TR1_MEMORY == 1
// shared pointers
#include <cstddef> // for __GLIBCXX__

#  ifdef __GLIBCXX__
#    include <tr1/memory>
#  else
#    ifdef __IBMCPP__
#      define __IBMCPP_TR1__
#    endif
#    include <memory>
#  endif
#  define USE_SHARED_PTR 1
#endif
#endif
])

# check for required functions and die if they are not found
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([popen pclose],,AC_MSG_ERROR([cannot use required functions]))


#############################################################################
# anything else
#############################################################################

# Force @builddir@ substitution. Without this src/Makefile.in lacked @builddir@
# substitution on gruenau.informatik.hu-berlin.de with automake 1.9.6.
AC_SUBST(builddir)

#############################################################################
# In the following, command line paraemters for the configure script are
# defined. Each parameter is also written into a Makefile conditional to
# later set compliler flags accordingly. Finally, the result is also written
# into the configuration header "config.h".
#############################################################################

AC_CONFIG_FILES([Makefile
                 libs/pnapi/Makefile
                 src/marlene.conf
                 src/Makefile
                 tests/Makefile
                 tests/package.m4
                 doc/Doxyfile
                 doc/Makefile
                 man/Makefile])
AC_CONFIG_FILES([tests/cover.sh], [chmod +x tests/cover.sh])
AC_OUTPUT


# display a warning in case a required tool has not been found
if test "${WENDY}" = ""; then
  echo
  echo "WARNING: Not all required tools have been found. Please read file"
  echo "         REQUIREMENTS and edit the file src/marlene.conf before"
  echo "         using or installing Marlene."
  echo
fi

