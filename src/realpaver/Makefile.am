###############################################################################
## This file is part of Realpaver, an interval constraint and NLP solver.    ##
##                                                                           ##
## Copyright (c) 2017-2023 LS2N, Nantes                                      ##
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
		ParamFile

lib_LTLIBRARIES = librealpaver.la

librealpaver_la_includedir=$(includedir)/realpaver

librealpaver_la_include_HEADERS = \
		Bitset.hpp \
		BC3Contractor.hpp \
		BC4Contractor.hpp \
		CIDContractor.hpp \
		Common.hpp \
		config.hpp \
		Constraint.hpp \
		ConstraintContractor.hpp \
		ConstraintFixer.hpp \
		ConstraintRelaxor.hpp \
		Contractor.hpp \
		ContractorPool.hpp \
		Dag.hpp \
		Double.hpp \
		Env.hpp \
		Exception.hpp \
		HC4Contractor.hpp \
		Inflator.hpp \
		IntContractor.hpp \
		Integer.hpp \
		Interval.hpp \
		IntervalFunction.hpp \
		IntervalFunctionVector.hpp \
		IntervalGaussSeidel.hpp \
		IntervalImpl.hpp \
		IntervalMatrix.hpp \
		IntervalNewton.hpp \
		IntervalRegion.hpp \
		IntervalSlicer.hpp \
		IntervalTraits.hpp \
		IntervalVector.hpp \
		IntervalUnion.hpp \
		LinumDoubleTraits.hpp \
		LinumIntervalTraits.hpp \
		LinumTraits.hpp \
		ListContractor.hpp \
		Logger.hpp \
		LPModel.hpp \
		LPSolver.hpp \
		MaxCIDContractor.hpp \
		NcspEnv.hpp \
		NcspNode.hpp \
		NcspSpace.hpp \
		NcspSpaceBFS.hpp \
		NcspSpaceDFS.hpp \
		NcspSpaceDMDFS.hpp \
		NcspSpaceHybridDFS.hpp \
		NcspSolver.hpp \
		NcspSplit.hpp \
		NumericMatrix.hpp \
		NumericVector.hpp \
		Objective.hpp \
		Param.hpp \
		Parser.hpp \
		parser_bison.hpp \
		PolytopeHullContractor.hpp \
		Preprocessor.hpp \
		Problem.hpp \
		Propagator.hpp \
		QuadraticTerm.hpp \
		Range.hpp \
		RangeSlicer.hpp \
		RealFunction.hpp \
		RealFunctionVector.hpp \
		RealMatrix.hpp \
		RealPoint.hpp \
		RealVector.hpp \
		RLTRelaxation.hpp \
		Scope.hpp \
		ScopeMap.hpp \
		SearchNode.hpp \
		SplitStrategy.hpp \
		Stat.hpp \
		SymbolTable.hpp \
		Term.hpp \
		TermDeriver.hpp \
		TermFixer.hpp \
		ThickFunction.hpp \
		Timer.hpp \
		Tolerance.hpp \
		UniIntervalNewton.hpp \
		Variable.hpp \
		VariableSelector.hpp \
		VariableVector.hpp

#		BOContractor.hpp \
#		BOLocalGradient.hpp \
#		BOLocalSolver.hpp \
#		BOModel.hpp \
#		BONode.hpp \
#		BOPresolver.hpp \
#		BOSolver.hpp \
#		BOSpace.hpp \
#		BOSplit.hpp

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
		CIDContractor.cpp \
		Common.cpp \
		Constraint.cpp \
		ConstraintContractor.cpp \
		ConstraintFixer.cpp \
		ConstraintRelaxor.cpp \
		Contractor.cpp \
		ContractorPool.cpp \
		Dag.cpp \
		Env.cpp \
		Exception.cpp \
		HC4Contractor.cpp \
		Inflator.cpp \
		IntContractor.cpp \
		Integer.cpp \
		Interval.cpp \
		IntervalFunction.cpp \
		IntervalFunctionVector.cpp \
		IntervalGaussSeidel.cpp \
		IntervalMatrix.cpp \
		IntervalNewton.cpp \
		IntervalRegion.cpp \
		IntervalSlicer.cpp \
		IntervalVector.cpp \
		IntervalUnion.cpp \
		ListContractor.cpp \
		Logger.cpp \
		LPModel.cpp \
		MaxCIDContractor.cpp \
		NcspEnv.cpp \
		NcspNode.cpp \
		NcspSpace.cpp \
		NcspSpaceDFS.cpp \
		NcspSpaceBFS.cpp \
		NcspSpaceDMDFS.cpp \
		NcspSpaceHybridDFS.cpp \
		NcspSolver.cpp \
		NcspSplit.cpp \
		Objective.cpp \
		Param.cpp \
		parser_bison.ypp \
		parser_flex.lpp \
		Parser.cpp \
		PolytopeHullContractor.cpp \
		Preprocessor.cpp \
		Problem.cpp \
		Propagator.cpp \
		QuadraticTerm.cpp \
		Range.cpp \
		RangeSlicer.cpp \
		RealFunction.cpp \
		RealFunctionVector.cpp \
		RealMatrix.cpp \
		RealPoint.cpp \
		RealVector.cpp \
		RLTRelaxation.cpp \
		Scope.cpp \
		ScopeMap.cpp \
		SearchNode.cpp \
		Stat.cpp \
		SymbolTable.cpp \
		Term.cpp \
		TermDeriver.cpp \
		TermFixer.cpp \
		ThickFunction.cpp \
		Timer.cpp \
		Tolerance.cpp \
		UniIntervalNewton.cpp \
		Variable.cpp \
		VariableSelector.cpp \
		VariableVector.cpp

#		BOContractor.cpp \
#		BOLocalGradient.cpp \
#		BOLocalSolver.cpp \
#		BOModel.cpp \
#		BONode.cpp \
#		BOPresolver.cpp \
#		BOSolver.cpp \
#		BOSpace.cpp \
#		BOSplit.cpp

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
