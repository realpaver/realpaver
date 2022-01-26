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
		box.hpp \
		common.hpp \
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
		inflator.hpp \
		interval.hpp \
		interval_slicer.hpp \
		interval_union.hpp \
		lp_model.hpp \
		newton.hpp \
		number.hpp \
		obj.hpp \
		param.hpp \
		parser.hpp \
		parser_bison.hpp \
		point.hpp \
		problem.hpp \
		propagator.hpp \
		real_fun.hpp \
		scope.hpp \
		scope_map.hpp \
		stopping.hpp \
		term.hpp \
		term_deriver.hpp \
		term_fixer.hpp \
		thick_fun.hpp \
		timer.hpp \
		uni_fun.hpp \
		variable.hpp \
		wrapper_interval_gaol.hpp

if COND_LP_CLP
librealpaver_la_include_HEADERS += wrapper_lpsolver_clp.hpp
endif

if COND_LP_GUROBI
librealpaver_la_include_HEADERS += wrapper_lpsolver_gurobi.hpp
endif

librealpaver_la_SOURCES = \
		bco_dag.cpp \
		bco_local.cpp \
		bco_model.cpp \
		bco_node.cpp \
		bco_space.cpp \
		bitset.cpp \
		box.cpp \
		common.cpp \
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
		inflator.cpp \
		interval.cpp \
		interval_slicer.cpp \
		interval_union.cpp \
		lp_model.cpp \
		newton.cpp \
		obj.cpp \
		param.cpp \
		parser_bison.ypp \
		parser_flex.lpp \
		parser.cpp \
		point.cpp \
		problem.cpp \
		propagator.cpp \
		real_fun.cpp \
		scope.cpp \
		scope_map.cpp \
		stopping.cpp \
		term.cpp \
		term_deriver.cpp \
		term_fixer.cpp \
		timer.cpp \
		thick_fun.cpp \
		uni_fun.cpp \
		variable.cpp

if COND_LP_CLP
librealpaver_la_SOURCES += wrapper_lpsolver_clp.cpp
endif

if COND_LP_GUROBI
librealpaver_la_SOURCES += wrapper_lpsolver_gurobi.cpp
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
