###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2022 LS2N, Nantes                                      ##
##                                                                           ##
## Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   ##
## COPYING for information.                                                  ##
###############################################################################

AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I..

EXTRA_DIST = \
		configure.hpp \
		parser_flex.lpp \
		parser_bison.ypp \
		settings.txt

lib_LTLIBRARIES = librealpaver.la

librealpaver_la_includedir=$(includedir)/realpaver

librealpaver_la_include_HEADERS = \
		bco_local.hpp \
		Bitset.hpp \
		BOContractor.hpp \
		BOModel.hpp \
		BONode.hpp \
		BOPresolver.hpp \
		BOSolver.hpp \
		BOSpace.hpp \
		BOSplit.hpp \
		Common.hpp \
		config.hpp \
		constraint.hpp \
		ConstraintFixer.hpp \
		ConstraintRelaxor.hpp \
		Contractor.hpp \
		contractor_bc3.hpp \
		contractor_bc4.hpp \
		contractor_cid.hpp \
		contractor_int.hpp \
		contractor_pool.hpp \
		dag.hpp \
		Double.hpp \
		Exception.hpp \
		HC4Contractor.hpp \
		Inflator.hpp \
		interval.hpp \
		IntervalImpl.hpp \
		IntervalNewton.hpp \
		IntervalSlicer.hpp \
		IntervalTraits.hpp \
		IntervalVector.hpp \
		IntervalUnion.hpp \
		LinumDoubleTraits.hpp \
		LinumIntervalTraits.hpp \
		LinumTraits.hpp \
		Logger.hpp \
		LPModel.hpp \
		LPSolver.hpp \
		NumericVector.hpp \
		Objective.hpp \
		Param.hpp \
		parser.hpp \
		parser_bison.hpp \
		Preprocessor.hpp \
		Problem.hpp \
		propagator.hpp \
		RealFunction.hpp \
		RealVector.hpp \
		Scope.hpp \
		ScopeMap.hpp \
		SearchNode.hpp \
		SplitStrategy.hpp \
		term.hpp \
		TermDeriver.hpp \
		TermFixer.hpp \
		thick_fun.hpp \
		Timer.hpp \
		Tolerance.hpp \
		uni_fun.hpp \
		Variable.hpp

if COND_INTERVAL_GAOL
librealpaver_la_include_HEADERS += IntervalGaol.hpp
endif

if COND_LP_CLP
librealpaver_la_include_HEADERS += LPSolverClp.hpp
endif

if COND_LP_GUROBI
librealpaver_la_include_HEADERS += LPSolverGurobi.hpp
endif

librealpaver_la_SOURCES = \
		AssertDebug.hpp \
		bco_local.cpp \
		Bitset.cpp \
		BOContractor.cpp \
		BOModel.cpp \
		BONode.cpp \
		BOPresolver.cpp \
		BOSolver.cpp \
		BOSpace.cpp \
		BOSplit.cpp \
		Common.cpp \
		constraint.cpp \
		ConstraintFixer.cpp \
		ConstraintRelaxor.cpp \
		Contractor.cpp \
		contractor_bc3.cpp \
		contractor_bc4.cpp \
		contractor_cid.cpp \
		contractor_int.cpp \
		contractor_pool.cpp \
		dag.cpp \
		Exception.cpp \
		HC4Contractor.cpp \
		Inflator.cpp \
		interval.cpp \
		IntervalNewton.cpp \
		IntervalSlicer.cpp \
		IntervalVector.cpp \
		IntervalUnion.cpp \
		Logger.cpp \
		LPModel.cpp \
		Objective.cpp \
		Param.cpp \
		parser_bison.ypp \
		parser_flex.lpp \
		parser.cpp \
		Preprocessor.cpp \
		Problem.cpp \
		propagator.cpp \
		RealFunction.cpp \
		RealVector.cpp \
		Scope.cpp \
		ScopeMap.cpp \
		SearchNode.cpp \
		term.cpp \
		TermDeriver.cpp \
		TermFixer.cpp \
		Timer.cpp \
		thick_fun.cpp \
		Tolerance.cpp \
		uni_fun.cpp \
		Variable.cpp

if COND_LP_CLP
librealpaver_la_SOURCES += LPSolverClp.cpp
endif

if COND_LP_GUROBI
librealpaver_la_SOURCES += LPSolverGurobi.cpp
endif

librealpaver_la_DEPENDENCIES = parser_flex.lpp parser_bison.ypp

librealpaver_la_LIBADD = -lm

BUILT_SOURCES = parser_bison.hpp
AM_YFLAGS = -d -v
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

DISTCLEANFILES = *.~* *.o *.lo
CLEANFILES = *~
MAINTAINERCLEANFILES = *.~* *.o *.lo \
		parser_flex.cpp parser_bison.hpp parser_bison.cpp
