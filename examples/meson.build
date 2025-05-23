#-------------------------------------------------------------------------------
# Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
#              interval computations.
#-------------------------------------------------------------------------------
# Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
#                         France
# Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
#                         France
#-------------------------------------------------------------------------------
# Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
# file for information.
#-------------------------------------------------------------------------------
# Configuration file processed by the Meson Build system.
# Author: Raphaël Chenouard
#-------------------------------------------------------------------------------

lp1 = executable(
  'lp1',
  sources: 'lp1.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

lp2 = executable(
  'lp2',
  sources: 'lp2.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

if local_solver_dep.found()
  nlp = executable(
    'nlp',
    sources: 'nlp.cpp',
    link_with: realpaver_lib,
    include_directories: ['../src'] + incdir,
    dependencies: deps_lib,
  )
endif

csp = executable(
  'csp',
  sources: 'csp.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

parsing = executable(
  'parsing',
  sources: 'parsing.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

propag = executable(
  'propag',
  sources: 'propag.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

uni_newton = executable(
  'uni_newton',
  sources: 'uni_newton.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

solve_2r_v1 = executable(
  'solve_2r_v1',
  sources: 'solve_2r_v1.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

solve_2r_v2 = executable(
  'solve_2r_v2',
  sources: 'solve_2r_v2.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src'] + incdir,
  dependencies: deps_lib,
)

# TODO
# Sub-directories
# subdir('ncsp')
