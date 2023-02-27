###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2023 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

AUTOMAKE_OPTIONS = foreign

SUBDIRS = bop ncsp
DIST_SUBDIRS = bop ncsp

LDADD = ../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@

AM_CPPFLAGS = -I.. -I../src

noinst_PROGRAMS = lp uni_newton propag

lp_SOURCES = lp.cpp
uni_newton_SOURCES = uni_newton.cpp
propag_SOURCES = propag.cpp

CLEANFILES = *~ *.log $(noinst_PROGRAMS)
