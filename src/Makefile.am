# This file is part of Realpaver. License: see COPYING file.

AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
		realpaver_configure.hpp \
		realpaver_flex.lpp \
		realpaver_bison.ypp

lib_LTLIBRARIES = librealpaver.la

librealpaver_la_includedir=$(includedir)/realpaver

librealpaver_la_include_HEADERS = \
		realpaver_bco_dag.hpp \
		realpaver_bco_local.hpp \
		realpaver_bco_model.hpp \
		realpaver_bco_node.hpp \
		realpaver_bco_space.hpp \
		realpaver_bison.hpp \
		realpaver_bitset.hpp \
		realpaver_box.hpp \
		realpaver_common.hpp \
		realpaver_config.hpp \
		realpaver_constraint.hpp \
		realpaver_constraint_fixer.hpp \
		realpaver_constraint_relaxor.hpp \
		realpaver_contractor.hpp \
		realpaver_contractor_bc3.hpp \
		realpaver_contractor_bc4.hpp \
		realpaver_contractor_bco.hpp \
		realpaver_contractor_cid.hpp \
		realpaver_contractor_hc4.hpp \
		realpaver_contractor_int.hpp \
		realpaver_contractor_pool.hpp \
		realpaver_dag.hpp \
		realpaver_double.hpp \
		realpaver_inflator.hpp \
		realpaver_interval.hpp \
		realpaver_interval_gaol.hpp \
		realpaver_interval_slicer.hpp \
		realpaver_interval_union.hpp \
		rpo_lin_model.hpp \
		realpaver_newton.hpp \
		realpaver_obj.hpp \
		realpaver_param.hpp \
		realpaver_parser.hpp \
		realpaver_point.hpp \
		realpaver_problem.hpp \
		realpaver_propagator.hpp \
		realpaver_real_fun.hpp \
		realpaver_scope.hpp \
		realpaver_scope_map.hpp \
		realpaver_stopping.hpp \
		realpaver_term.hpp \
		realpaver_term_deriver.hpp \
		realpaver_term_fixer.hpp \
		realpaver_thick_fun.hpp \
		realpaver_timer.hpp \
		realpaver_uni_fun.hpp \
		realpaver_variable.hpp


librealpaver_la_SOURCES = \
		realpaver_bco_dag.cpp \
		realpaver_bco_local.cpp \
		realpaver_bco_model.cpp \
		realpaver_bco_node.cpp \
		realpaver_bco_space.cpp \
		realpaver_bison.ypp \
		realpaver_flex.lpp \
		realpaver_bitset.cpp \
		realpaver_box.cpp \
		realpaver_common.cpp \
		realpaver_constraint.cpp \
		realpaver_constraint_fixer.cpp \
		realpaver_constraint_relaxor.cpp \
		realpaver_contractor.cpp \
		realpaver_contractor_bc3.cpp \
		realpaver_contractor_bc4.cpp \
		realpaver_contractor_bco.cpp \
		realpaver_contractor_cid.cpp \
		realpaver_contractor_hc4.cpp \
		realpaver_contractor_int.cpp \
		realpaver_contractor_pool.cpp \
		realpaver_dag.cpp \
		realpaver_inflator.cpp \
		realpaver_interval.cpp \
		realpaver_interval_slicer.cpp \
		realpaver_interval_union.cpp \
		rpo_lin_model.cpp \
		realpaver_newton.cpp \
		realpaver_obj.cpp \
		realpaver_param.cpp \
		realpaver_parser.cpp \
		realpaver_point.cpp \
		realpaver_problem.cpp \
		realpaver_propagator.cpp \
		realpaver_real_fun.cpp \
		realpaver_scope.cpp \
		realpaver_scope_map.cpp \
		realpaver_stopping.cpp \
		realpaver_term.cpp \
		realpaver_term_deriver.cpp \
		realpaver_term_fixer.cpp \
		realpaver_timer.cpp \
		realpaver_thick_fun.cpp \
		realpaver_uni_fun.cpp \
		realpaver_variable.cpp

if COND_LP_CLP
librealpaver_la_include_HEADERS += rpo_simplex_clp.hpp
librealpaver_la_SOURCES += rpo_simplex_clp.cpp
endif

if COND_LP_GUROBI
librealpaver_la_include_HEADERS += rpo_simplex_gurobi.hpp
librealpaver_la_SOURCES += rpo_simplex_gurobi.cpp
endif


librealpaver_la_DEPENDENCIES = realpaver_flex.lpp realpaver_bison.ypp

librealpaver_la_LIBADD = -lm

BUILT_SOURCES = realpaver_bison.hpp
AM_YFLAGS = -d -v
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

DISTCLEANFILES = *.~* *.o *.lo
CLEANFILES = *~
MAINTAINERCLEANFILES = *.~* *.o *.lo \
		realpaver_flex.cpp realpaver_bison.hpp realpaver_bison.cpp
