AUTOMAKE_OPTIONS = foreign

LDADD = ../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@

AM_CPPFLAGS = -I.. -I../src

CLEANFILES = *~

noinst_PROGRAMS = interval_newton lp

interval_newton_SOURCES = interval_newton.cpp
lp_SOURCES = lp.cpp
