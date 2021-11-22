# This file is part of Realpaver. License: see COPYING file.

AUTOMAKE_OPTIONS = foreign

LDADD = -lrealpaver @INTERVAL_LIB@ @CPPUNIT_LIB@
AM_CPPFLAGS = -I../src
AM_LDFLAGS = -L../src/.libs

CLEANFILES = *~

if CPPUNIT_COND
check_PROGRAMS = \
	bitset_test \
	dag_test \
	hc4revise_test \
	interval_test

bitset_test_SOURCES = bitset_test.cpp
dag_test_SOURCES = dag_test.cpp
hc4revise_test_SOURCES = hc4revise_test.cpp
interval_test_SOURCES = interval_test.cpp
endif

EXTRA_DIST = realpaver_test.hpp

TESTS = $(check_PROGRAMS)
