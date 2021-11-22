# This file is part of Realpaver. License: see COPYING file.

AUTOMAKE_OPTIONS = foreign

LDADD = -lrealpaver @INTERVAL_LIB@
AM_CPPFLAGS = -I../src
AM_LDFLAGS = -L../src/.libs

CLEANFILES = *~

noinst_PROGRAMS = work

work_SOURCES = work.cpp
