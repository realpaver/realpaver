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
		Bitset.hpp \
		BC3Contractor.hpp \
		BC4Contractor.hpp \
		BOContractor.hpp \
		BOLocalConjugate.hpp \
		BOLocalSolver.hpp \
		BOModel.hpp \
		BONode.hpp \
		BOPresolver.hpp \
		BOSolver.hpp \
		BOSpace.hpp \
		BOSplit.hpp \
		CIDContractor.hpp \
		Common.hpp \
		config.hpp \
		Constraint.hpp \
		ConstraintFixer.hpp \
		ConstraintRelaxor.hpp \
		Contractor.hpp \
		ContractorPool.hpp \
		dag.hpp \
		Double.hpp \
		Exception.hpp \
		HC4Contractor.hpp \
		Inflator.hpp \
		IntContractor.hpp \
		Interval.hpp \
		IntervalFunction.hpp \
		IntervalImpl.hpp \
		IntervalNewton.hpp \
		IntervalRegion.hpp \
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
		MaxCIDContractor.hpp \
		NumericVector.hpp \
		Objective.hpp \
		Param.hpp \
		parser.hpp \
		parser_bison.hpp \
		Preprocessor.hpp \
		Problem.hpp \
		Propagator.hpp \
		RealFunction.hpp \
		RealPoint.hpp \
		RealVector.hpp \
		Scope.hpp \
		ScopeMap.hpp \
		SearchNode.hpp \
		Selector.hpp \
		SplitStrategy.hpp \
		Term.hpp \
		TermDeriver.hpp \
		TermFixer.hpp \
		ThickIntervalFunction.hpp \
		Timer.hpp \
		Tolerance.hpp \
		UniIntervalFunction.hpp \
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
		Bitset.cpp \
		BC3Contractor.cpp \
		BC4Contractor.cpp \
		BOContractor.cpp \
		BOLocalConjugate.cpp \
		BOLocalSolver.cpp \
		BOModel.cpp \
		BONode.cpp \
		BOPresolver.cpp \
		BOSolver.cpp \
		BOSpace.cpp \
		BOSplit.cpp \
		CIDContractor.cpp \
		Common.cpp \
		Constraint.cpp \
		ConstraintFixer.cpp \
		ConstraintRelaxor.cpp \
		Contractor.cpp \
		ContractorPool.cpp \
		dag.cpp \
		Exception.cpp \
		HC4Contractor.cpp \
		Inflator.cpp \
		IntContractor.cpp \
		Interval.cpp \
		IntervalFunction.cpp \
		IntervalNewton.cpp \
		IntervalRegion.cpp \
		IntervalSlicer.cpp \
		IntervalVector.cpp \
		IntervalUnion.cpp \
		Logger.cpp \
		LPModel.cpp \
		MaxCIDContractor.cpp \
		Objective.cpp \
		Param.cpp \
		parser_bison.ypp \
		parser_flex.lpp \
		parser.cpp \
		Preprocessor.cpp \
		Problem.cpp \
		Propagator.cpp \
		RealFunction.cpp \
		RealPoint.cpp \
		RealVector.cpp \
		Scope.cpp \
		ScopeMap.cpp \
		SearchNode.cpp \
		Selector.cpp \
		Term.cpp \
		TermDeriver.cpp \
		TermFixer.cpp \
		Timer.cpp \
		ThickIntervalFunction.cpp \
		Tolerance.cpp \
		UniIntervalFunction.cpp \
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
