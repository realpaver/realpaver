###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2023 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

AUTOMAKE_OPTIONS = foreign

SUBDIRS = generators solved unsolved
DIST_SUBDIRS = generators solved unsolved

LDADD = ../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@

AM_CPPFLAGS = -I.. -I../src

CLEANFILES = *~ *.log

noinst_PROGRAMS = ncsp_solver

ncsp_solver_SOURCES = ncsp_solver.cpp

EXTRA_DIST = \
	ncsp_param
