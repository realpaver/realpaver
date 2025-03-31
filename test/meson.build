# ------------------------------------------------------------------------------
# Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
#              interval computations.
# ------------------------------------------------------------------------------
# Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
#                         France
# Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
#                         France
# ------------------------------------------------------------------------------
# Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
# file for information.
# ------------------------------------------------------------------------------
# Configuration file processed by the Meson Build system.
# Author: Raphaël Chenouard
# ------------------------------------------------------------------------------

affine_test = executable(
  'affine_test',
  sources: 'affine_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of affine form', affine_test)

affine_creator_test = executable(
  'affine_creator_test',
  sources: 'affine_creator_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of creator of affine forms', affine_creator_test)

bitset_test = executable(
  'bitset_test',
  sources: 'bitset_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of bitset', bitset_test)

ctc_hc4_test = executable(
  'ctc_hc4_test',
  sources: 'ctc_hc4_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of HC4 contractor', ctc_hc4_test)

ctc_loop_test = executable(
  'ctc_loop_test',
  sources: 'ctc_loop_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of loop contractor', ctc_loop_test)

ctc_polytope_test = executable(
  'ctc_polytope_test',
  sources: 'ctc_polytope_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of polytope hull contractor', ctc_polytope_test)

ctc_var_3b_test = executable(
  'ctc_var_3b_test',
  sources: 'ctc_var_3b_test.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'],
)
test('Test of var3B contractor', ctc_var_3b_test)

ctc_var_cid_test = executable(
  'ctc_var_cid_test',
  sources: 'ctc_var_cid_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of varCID contractor', ctc_var_cid_test)

dag_test = executable(
  'dag_test',
  sources: 'dag_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of dag', dag_test)

integer_test = executable(
  'integer_test',
  sources: 'integer_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of integer', integer_test)

interval_test = executable(
  'interval_test',
  sources: 'interval_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of interval', interval_test)

flat_function_test = executable(
  'flat_function_test',
  sources: 'flat_function_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of flat function', flat_function_test)

linearizer_test = executable(
  'linearizer_test',
  sources: 'linearizer_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of linearization methods', linearizer_test)

lp_solver_test = executable(
  'lp_solver_test',
  sources: 'lp_solver_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of LP solver', lp_solver_test)

preprocessor_test = executable(
  'preprocessor_test',
  sources: 'preprocessor_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of preprocessor', preprocessor_test)

range_test = executable(
  'range_test',
  sources: 'range_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of range', range_test)

real_matrix_test = executable(
  'real_matrix_test',
  sources: 'real_matrix_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of real matrix', real_matrix_test)

scope_test = executable(
  'scope_test',
  sources: 'scope_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of scope', scope_test)

real_function_vector_test = executable(
  'real_function_vector_test',
  sources: 'real_function_vector_test.cpp',
  link_with: realpaver_lib,
  dependencies: deps_lib,
  include_directories: ['../src'],
)
test('Test of real function vector', real_function_vector_test)

if local_solver_dep.found()
  nlp_solver_test = executable(
    'nlp_solver_test',
    sources: 'nlp_solver_test.cpp',
    link_with: realpaver_lib,
    dependencies: deps_lib,
    include_directories: ['../src'],
  )
  test('Test of NLP solver API', nlp_solver_test)
endif
