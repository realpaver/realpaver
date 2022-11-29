###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2022 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

AUTOMAKE_OPTIONS = foreign

LDADD = ../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@ @CPPUNIT_LIB@
AM_CPPFLAGS = -I.. -I../src
AM_LDFLAGS = -L../src/realpaver/.libs

CLEANFILES = *~

if COND_CPPUNIT
check_PROGRAMS = \
	bitset_test \
	dag_test \
	hc4revise_test \
	integer_test \
	interval_test \
	range_test \
	scope_test

bitset_test_SOURCES = bitset_test.cpp
dag_test_SOURCES = dag_test.cpp
hc4revise_test_SOURCES = hc4revise_test.cpp
integer_test_SOURCES = integer_test.cpp
interval_test_SOURCES = interval_test.cpp
range_test_SOURCES = range_test.cpp
scope_test_SOURCES = scope_test.cpp
endif

EXTRA_DIST = realpaver_test.hpp

TESTS = $(check_PROGRAMS)
