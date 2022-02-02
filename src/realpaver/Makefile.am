# This file is part of Realpaver. License: see COPYING file.

AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I..

EXTRA_DIST = \
		configure.hpp \
		parser_flex.lpp \
		parser_bison.ypp

lib_LTLIBRARIES = librealpaver.la

librealpaver_la_includedir=$(includedir)/realpaver

librealpaver_la_include_HEADERS = \
		bco_dag.hpp \
		bco_local.hpp \
		bco_model.hpp \
		bco_node.hpp \
		bco_space.hpp \
		bitset.hpp \
		Common.hpp \
		config.hpp \
		constraint.hpp \
		constraint_fixer.hpp \
		constraint_relaxor.hpp \
		contractor.hpp \
		contractor_bc3.hpp \
		contractor_bc4.hpp \
		contractor_bco.hpp \
		contractor_cid.hpp \
		contractor_hc4.hpp \
		contractor_int.hpp \
		contractor_pool.hpp \
		dag.hpp \
		Double.hpp \
		Exception.hpp \
		Inflator.hpp \
		interval.hpp \
		IntervalImpl.hpp \
		interval_newton.hpp \
		interval_slicer.hpp \
		IntervalTraits.hpp \
		IntervalVector.hpp \
		interval_union.hpp \
		LinumDoubleTraits.hpp \
		LinumIntervalTraits.hpp \
		LinumTraits.hpp \
		Logger.hpp \
		LPModel.hpp \
		LPSolver.hpp \
		NumericVector.hpp \
		obj.hpp \
		param.hpp \
		parser.hpp \
		parser_bison.hpp \
		problem.hpp \
		propagator.hpp \
		real_fun.hpp \
		RealVector.hpp \
		scope.hpp \
		scope_map.hpp \
		term.hpp \
		term_deriver.hpp \
		term_fixer.hpp \
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
		bco_dag.cpp \
		bco_local.cpp \
		bco_model.cpp \
		bco_node.cpp \
		bco_space.cpp \
		bitset.cpp \
		Common.cpp \
		constraint.cpp \
		constraint_fixer.cpp \
		constraint_relaxor.cpp \
		contractor.cpp \
		contractor_bc3.cpp \
		contractor_bc4.cpp \
		contractor_bco.cpp \
		contractor_cid.cpp \
		contractor_hc4.cpp \
		contractor_int.cpp \
		contractor_pool.cpp \
		dag.cpp \
		Exception.cpp \
		Inflator.cpp \
		interval.cpp \
		interval_newton.cpp \
		interval_slicer.cpp \
		IntervalVector.cpp \
		interval_union.cpp \
		Logger.cpp \
		LPModel.cpp \
		obj.cpp \
		param.cpp \
		parser_bison.ypp \
		parser_flex.lpp \
		parser.cpp \
		problem.cpp \
		propagator.cpp \
		real_fun.cpp \
		RealVector.cpp \
		scope.cpp \
		scope_map.cpp \
		term.cpp \
		term_deriver.cpp \
		term_fixer.cpp \
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
