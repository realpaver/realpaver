# This file is part of Realpaver. License: see COPYING file.

AUTOMAKE_OPTIONS = foreign

LDADD = ../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@

AM_CPPFLAGS = -I.. -I../src

CLEANFILES = *~

noinst_PROGRAMS = complex work

work_SOURCES = work.cpp
complex_SOURCES = complex.cpp
