# This file is part of Realpaver. License: see COPYING file.

m4_define([realpaver_name], [realpaver])
m4_define([realpaver_version], [1.0])

AC_INIT([realpaver_name],
        [realpaver_version],
        [laurent.granvilliers@univ-nantes.fr])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/realpaver_config.hpp])
AC_CONFIG_HEADERS([src/realpaver_configure.hpp])
AC_CONFIG_MACRO_DIR([m4])


# programs
AC_PROG_CC
AC_PROG_CXX
CXXFLAGS="-I. -std=c++14 -frounding-math -Wall -pedantic"
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S
LT_INIT


# flex / bison
AM_PROG_LEX
AC_PROG_YACC


# host architecture
AC_CANONICAL_HOST

AC_MSG_CHECKING([for host type])
AC_MSG_RESULT([$host])

AC_MSG_CHECKING([for host processor])
case "$host_cpu" in
i*86)
   have_x86=yes
   AC_DEFINE(ARCH_X86, 1, [Define to 1 if you are compiling for ix86.])
   AC_MSG_RESULT([x86])
   ;;
x86_64)
   have_x86=yes
    AC_DEFINE(ARCH_X86, 1, [Define to 1 if you are compiling for ix86.])
    AC_DEFINE(ARCH_X86_64, 1, [Define to 1 if you are compiling for amd64.])
    AC_MSG_RESULT([x86-64])
   ;;
*)
   AC_MSG_RESULT([unknown])
   ;;
esac

# Linux ?
AC_MSG_CHECKING([if compiling for Linux])
case "$host_os" in
linux*)
   platform_linux=yes
   ;;
*)
   platform_linux=no
   ;;
esac
AC_MSG_RESULT([$platform_linux])
AM_CONDITIONAL(PLATFORM_LINUX, test "$platform_linux" = "yes")

# Mac OS X ?
AC_MSG_CHECKING([if compiling for Mac OS X])
case "$host_os" in
darwin*)
   AC_MSG_RESULT(yes)
   AC_DEFINE(PLATFORM_OSX, 1, [Define to 1 if compiling for Mac OS X])
   platform_osx=yes
   ;;
*)
   platform_osx=no
   ;;
esac
AC_MSG_RESULT($platform_osx)
AM_CONDITIONAL(PLATFORM_OSX, test "$platform_osx" = "yes")


# --with-itvlib-include
AC_ARG_WITH([include-path],
            [AS_HELP_STRING([--with-include-path],
                            [Specify an include path for third-party headers])],
            incpath="-I${with_include_path}",
            incpath="")
CPPFLAGS="$CPPFLAGS ${incpath}"


# --with-lib-path
AC_ARG_WITH([lib_path],
            [AS_HELP_STRING([--with-lib-path],
            [Specify a library path for third-party libraries])],
            libpath="-L${with_lib_path}",
            libpath="")
CXXFLAGS="$CXXFLAGS ${libpath}"

# --with-itvlib
AC_ARG_WITH([itvlib],
            [AS_HELP_STRING([--with-itvlib=(gaol)],
            [Specify the interval library to use @<:@default=gaol@:>@])],
            with_itvlib="${with_itvlib}",
            with_itvlib="gaol")

interval_library="no"


# interval library

# gaol with apmathlib
if test "${with_itvlib}" = "gaol"; then
   AC_LANG_PUSH([C++])
   AC_MSG_CHECKING([for gaol headers])
   AC_COMPILE_IFELSE([
                        AC_LANG_PROGRAM( [#include <gaol/gaol_config.h>] )
                     ],
                     [have_gaol_headers=yes],
                     [AC_MSG_ERROR("gaol headers not found")] )
   AC_MSG_RESULT(${have_gaol_headers})

   OLD_LIBS=$LIBS
   LIBS="-lgaol -lultim -lm"

   AC_MSG_CHECKING([for libgaol])
   AC_RUN_IFELSE( [
                     AC_LANG_PROGRAM( [ #include <gaol/gaol> ],
                                       [ gaol::interval itv;  ] )
                  ],
                  [have_gaol=yes],
                  [have_gaol=no])
   AC_MSG_RESULT(${have_gaol})

   if test ${have_gaol} = "yes"; then
      interval_library="gaol"
      INTERVAL_LIB=$LIBS
      AC_DEFINE(INTERVAL_LIB_GAOL,1,
            [Define to 1 to use the Gaol interval arithmetic library.])
      AC_CONFIG_LINKS([src/realpaver_interval_impl.hpp:src/realpaver_interval_gaol.hpp])
   else
      AC_MSG_ERROR("gaol library not found")
   fi

   LIBS=$OLD_LIBS
   AC_LANG_POP([C++])
fi

if test ${interval_library} = "no"; then
   AC_MSG_ERROR("interval arithmetic library not found")   
fi

AC_SUBST(INTERVAL_LIB)


# cppUnit
AC_LANG_PUSH([C++])
OLD_LIBS=$LIBS
LIBS="$dllib -lcppunit $LIBS"
AC_MSG_CHECKING([for libcppunit])
AC_RUN_IFELSE([
            AC_LANG_PROGRAM([ #include <cppunit/ui/text/TestRunner.h> ],
                            [ CppUnit::TextUi::TestRunner runner;     ])
               ],
           [have_cppunit=yes],
           [have_cppunit=no ])
AC_MSG_RESULT(${have_cppunit})
AM_CONDITIONAL([CPPUNIT_COND],[test ${have_cppunit} = "yes"])
if test "${have_cppunit}" = "yes"; then
   CPPUNIT_LIB=$LIBS
else
   CPPUNIT_LIB=""
fi
LIBS=$OLD_LIBS
AC_LANG_POP([C++])

AC_SUBST(CPPUNIT_LIB)


# --enable-debug
AC_MSG_CHECKING([whether to turn on debugging])
AC_ARG_ENABLE(debug,
   [AS_HELP_STRING([--enable-debug],
                   [turn on debugging (default=no)])],
   [enable_debug=yes],
   [enable_debug=no])

if test ${enable_debug} = "yes"; then
   CXXFLAGS="$CXXFLAGS -g -O0"
   AC_DEFINE(DEBUG_ON,1,
             [Define to 1 to turn on debugging.])
   AC_MSG_RESULT([yes])
else
   CXXFLAGS="$CXXFLAGS -O3 -funroll-loops -fomit-frame-pointer"
   AC_DEFINE(DEBUG_ON,0,
             [Define this to 1 to turn on debugging.])
   AC_MSG_RESULT([no])
fi


# --enable-log
AC_MSG_CHECKING([whether to generate log files])
AC_ARG_ENABLE(log,
   [AS_HELP_STRING([--enable-log],
                   [generate log files (default=no)])],
   [enable_log=yes],
   [enable_log=no])

if test ${enable_log} = "yes"; then
   AC_DEFINE(LOG_ON,1,
             [Define to 1 to generate log files.])
   AC_MSG_RESULT([yes])
else
   AC_DEFINE(LOG_ON,0,
             [Define this to 1 to generate log files.])
   AC_MSG_RESULT([no])
fi


# --enable-assert
AC_MSG_CHECKING([whether to check assertions])
AC_ARG_ENABLE(assert,
   [AS_HELP_STRING([--enable-assert],
                   [check assertions (default=no)])],
   [enable_assert=yes],
   [enable_assert=no])

if test ${enable_assert} = "yes"; then
   AC_DEFINE(ASSERT_ON,1,
             [Define to 1 to check assertions.])
   AC_MSG_RESULT([yes])
else
   AC_DEFINE(ASSERT_ON,0,
             [Define this to 1 to check assertions.])
   AC_MSG_RESULT([no])
fi


# file generation
AC_CONFIG_FILES([Makefile
                 check/Makefile
                 src/Makefile
                 work/Makefile])
AC_OUTPUT


# options display
echo ""
printf "Realpaver v.realpaver_version configured\n"
printf "\tinterval lib       : $interval_library\n"
printf "\tdebugging          : $enable_debug\n"
printf "\tlog files          : $enable_log\n"
printf "\tassertion checking : $enable_log\n"
printf "\tunit testing       : $have_cppunit\n"
printf "\nRun make to compile Realpaver\n"
echo ""


# Gaol configuration
# ./configure --prefix=/home/granvilliers-l/local/ --with-mathlib-include=/home/granvilliers-l/local/include --with-mathlib-lib=/home/granvilliers-l/local/lib --with-mathlib=apmathlib --enable-preserve-rounding=yes --enable-verbose-mode=no

# Realpaver configuration
# ./configure --prefix=/home/granvilliers-l/local/ --with-include-path=/home/granvilliers-l/local/include --with-lib-path=/home/granvilliers-l/local/lib --with-itvlib=gaol --enable-debug --enable-log --enable-assert
