###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2022 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

AUTOMAKE_OPTIONS = foreign

LDADD = ../../src/realpaver/.libs/librealpaver.la @INTERVAL_LIB@ @LP_LIB@

AM_CPPFLAGS = -I.. -I../../src

CLEANFILES = *~ *.log

noinst_PROGRAMS = bop_solver

bop_solver_SOURCES = bop_solver.cpp

EXTRA_DIST = \
	bop_config \
	Beale.bop \
	Bukin.bop \
	Colville.bop \
	DropWave.bop \
	Eggholder.bop \
	GoldsteinPrice.bop \
	Hartmann_3.bop \
	McCormick.bop \
	Rastrigin.bop \
	Schwefel.bop \
	SixHumpCamel.bop \
	Shubert.bop \
	StyblinskiTang_2.bop \
	StyblinskiTang_10.bop \
	ThreeHumpCamel.bop
