###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2022 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

AUTOMAKE_OPTIONS = foreign

SUBDIRS = bop
DIST_SUBDIRS = bop

LDADD = ../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@

AM_CPPFLAGS = -I.. -I../src

CLEANFILES = *~ *.log

noinst_PROGRAMS = lp newton propag

lp_SOURCES = lp.cpp
newton_SOURCES = newton.cpp
propag_SOURCES = propag.cpp
