###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2023 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

m4_define([realpaver_name], [Realpaver])
m4_define([realpaver_version], [1.0])

AC_INIT([realpaver_name],
        [realpaver_version],
        [laurent.granvilliers@univ-nantes.fr])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/realpaver/config.hpp])
AC_CONFIG_HEADERS([src/realpaver/configure.hpp])
AC_CONFIG_MACRO_DIR([m4])

REALPAVER_VERSION=realpaver_version
AC_SUBST(REALPAVER_VERSION)


# programs
AC_PROG_CC
AC_PROG_CXX
CXXFLAGS="-I. -std=c++14 -frounding-math -Wall -pedantic"
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S
LT_INIT


# flex / bison
AC_PROG_LEX(noyywrap)
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


# --with-include-path
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
            [AS_HELP_STRING([--with-itvlib=(GaolApmathlib, GaolCrlibm)],
            [Specify the interval library to use @<:@default=GaolApmathlib@:>@])],
            use_itvlib="${with_itvlib}",
            use_itvlib="GaolApmathlib")

case "$use_itvlib" in
GaolApmathlib)
   name_itvlib="Gaol"
   gaol_mathlib="apmathlib"
;;
GaolCrlibm)
   name_itvlib="Gaol"
   gaol_mathlib="crlibm"
esac


# Gaol
if test "${name_itvlib}" = "Gaol"; then
   AC_LANG_PUSH([C++])
   AC_MSG_CHECKING([for Gaol headers])
   AC_COMPILE_IFELSE([
                        AC_LANG_PROGRAM( [#include <gaol/gaol_config.h>] )
                     ],
                     [have_gaol_headers=yes],
                     [AC_MSG_ERROR("Gaol headers not found")] )
   AC_MSG_RESULT(${have_gaol_headers})

   OLD_LIBS=$LIBS

   if test "${use_itvlib}" = "GaolApmathlib"; then
      LIBS="-lgaol -lultim -lm"
   else
      LIBS="-lgaol -lcrlibm -lm"
   fi

   AC_MSG_CHECKING([for libgaol])
   AC_RUN_IFELSE( [
                     AC_LANG_PROGRAM( [ #include <gaol/gaol> ],
                                       [ gaol::interval itv;  ] )
                  ],
                  [have_gaol=yes],
                  [have_gaol=no])
   AC_MSG_RESULT(${have_gaol})

   if test ${have_gaol} = "yes"; then
      INTERVAL_LIB=$LIBS
      AC_DEFINE(INTERVAL_LIB_GAOL, 1,
            [Define to 1 to use the Gaol interval arithmetic library.])
   else
      AC_MSG_ERROR("Gaol library not found")
   fi

   LIBS=$OLD_LIBS
   AC_LANG_POP([C++])
fi


AC_SUBST(INTERVAL_LIB)
AC_CONFIG_LINKS([src/realpaver/IntervalImpl.hpp:src/realpaver/Interval${name_itvlib}.hpp])
AM_CONDITIONAL([COND_INTERVAL_GAOL], [test ${name_itvlib} = "Gaol"])


# --with-lplib
AC_ARG_WITH([lplib],
            [AS_HELP_STRING([--with-lplib=(Clp, Gurobi)],
            [Specify the linear programming library to use @<:@default=Clp@:>@])],
            use_lplib="${with_lplib}",
            use_lplib="Clp")

# Gurobi
if test "${use_lplib}" = "Gurobi"; then
   AC_LANG_PUSH([C++])
   AC_MSG_CHECKING([for Gurobi headers])
   AC_COMPILE_IFELSE([
                        AC_LANG_PROGRAM( [#include <gurobi_c++.h>] )
                     ],
                     [have_gurobi_headers=yes],
                     [AC_MSG_ERROR("Gurobi headers not found")] )
   AC_MSG_RESULT(${have_gurobi_headers})
   OLD_LIBS=$LIBS
   LIBS="-lgurobi_c++ -lgurobi95"
   AC_MSG_CHECKING([for libgurobi_c++])
   AC_RUN_IFELSE( [
                     AC_LANG_PROGRAM( [ #include <gurobi_c++.h> ],
                                       [ GRBEnv env;  ] )
                  ],
                  [have_gurobi=yes],
                  [have_gurobi=no])
   AC_MSG_RESULT(${have_gurobi})
   if test ${have_gurobi} = "yes"; then
      LP_LIB=$LIBS
      AC_DEFINE(LP_LIB_GUROBI, 1,
            [Define to 1 to use Gurobi.])
   else
      AC_MSG_ERROR("Gurobi not found")
   fi

   LIBS=$OLD_LIBS
   AC_LANG_POP([C++])
fi

# Clp
if test "${with_lplib}" = "Clp"; then
   AC_LANG_PUSH([C++])
   AC_MSG_CHECKING([for CoinUtils headers])
   AC_COMPILE_IFELSE([
                        AC_LANG_PROGRAM( [#include <CoinParam.hpp>] )
                     ],
                     [have_coin_headers=yes],
                     [AC_MSG_ERROR("CoinUtils headers not found")] )
   AC_MSG_RESULT(${have_coin_headers})
   AC_MSG_CHECKING([for Clp headers])
   AC_COMPILE_IFELSE([
                        AC_LANG_PROGRAM( [#include <ClpSimplex.hpp>] )
                     ],
                     [have_clp_headers=yes],
                     [AC_MSG_ERROR("Clp headers not found")] )
   AC_MSG_RESULT(${have_clp_headers})
   OLD_LIBS=$LIBS
   LIBS="-lCoinUtils -lClp"
   AC_MSG_CHECKING([for libCoinUtils])
   AC_RUN_IFELSE( [
                     AC_LANG_PROGRAM( [ #include <CoinParam.hpp> ],
                                       [ CoinParam param;  ] )
                  ],
                  [have_coin=yes],
                  [have_coin=no])
   AC_MSG_RESULT(${have_coin})
   AC_MSG_CHECKING([for libClp])
   AC_RUN_IFELSE( [
                     AC_LANG_PROGRAM( [ #include <ClpSimplex.hpp> ],
                                       [ ClpSimplex simplex;  ] )
                  ],
                  [have_clp=yes],
                  [have_clp=no])
   AC_MSG_RESULT(${have_clp})
   if test ${have_clp} = "yes"; then
      LP_LIB=$LIBS
      AC_DEFINE(LP_LIB_CLP, 1,
            [Define to 1 to use the Clp solver.])
   else
      AC_MSG_ERROR("Clp not found")
   fi
   LIBS=$OLD_LIBS
   AC_LANG_POP([C++])
fi

AC_SUBST(LP_LIB)
AC_CONFIG_LINKS([src/realpaver/LPSolver.hpp:src/realpaver/LPSolver${use_lplib}.hpp])
AM_CONDITIONAL([COND_LP_GUROBI], [test ${use_lplib} = "Gurobi"])
AM_CONDITIONAL([COND_LP_CLP], [test ${use_lplib} = "Clp"])


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
AM_CONDITIONAL([COND_CPPUNIT],[test ${have_cppunit} = "yes"])
if test "${have_cppunit}" = "yes"; then
   CPPUNIT_LIB=$LIBS
else
   CPPUNIT_LIB=""
fi
LIBS=$OLD_LIBS
AC_LANG_POP([C++])

AC_SUBST(CPPUNIT_LIB)


# doxygen
AC_CHECK_PROG(DOXYGEN,doxygen,yes,no)
if test "${DOXYGEN}" = "no"; then
   AC_MSG_WARN("doxygen not found")
fi
AM_CONDITIONAL(DOXYGEN, test "${DOXYGEN}" = "yes")


# dot
AC_CHECK_PROG(DOT,dot,yes,no)
if test "${DOT}" = "no"; then
   AC_MSG_WARN("dot not found")
fi


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


# --enable-log
AC_MSG_CHECKING([whether to generate log files])
AC_ARG_ENABLE(log,
   [AS_HELP_STRING([--enable-log],
                   [generate log files (default=no)])],
   [enable_log=yes],
   [enable_log=no])

if test ${enable_log} = "yes"; then
   AC_DEFINE(LOG_ON, 1,
             [Define to 1 to generate log files.])
   AC_MSG_RESULT([yes])
else
   AC_DEFINE(LOG_ON, 0,
             [Define this to 1 to generate log files.])
   AC_MSG_RESULT([no])
fi


# file generation
AC_CONFIG_FILES([Makefile
                 check/Makefile
                 doc/Doxyfile
                 doc/Makefile
                 examples/Makefile
                 examples/bop/Makefile
                 ncsp/Makefile
                 ncsp/generators/Makefile
                 ncsp/solved/Makefile
                 ncsp/unsolved/Makefile
                 src/Makefile
                 src/realpaver/Makefile
                 work/Makefile])
AC_OUTPUT


# options display
echo ""
printf "Realpaver v. realpaver_version configured\n"
printf "\tInterval library   : ${name_itvlib}\n"
printf "\tLP solver          : ${use_lplib}\n"
printf "\tDebugging          : ${enable_debug}\n"
printf "\tAssertion checking : ${enable_assert}\n"
printf "\tLog files          : ${enable_log}\n"
printf "\tUnit testing       : ${have_cppunit}\n"
printf "\tDoxygen            : ${DOXYGEN}\n"
printf "\nRun make to compile Realpaver\n"
echo ""


# Gaol configuration
# ./configure --prefix=/home/granvilliers-l/local/ --with-mathlib-include=/home/granvilliers-l/local/include --with-mathlib-lib=/home/granvilliers-l/local/lib --with-mathlib=apmathlib --enable-preserve-rounding=yes --enable-verbose-mode=no

# Realpaver configuration
# ./configure --prefix=/usr/local/ --with-include-path=/usr/local/include --with-lib-path=/usr/local/lib --with-itvlib=GaolCrlibm --with-lplib=Clp --enable-debug --enable-assert --enable-log
