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

rp_gen_params = executable(
  'rp_gen_params',
  sources: 'rp_gen_params.cpp',
  link_with: realpaver_lib,
  include_directories: incdir,
  install: true,
  dependencies: deps_lib,
)

rp_default_params = custom_target(
  command: [rp_gen_params, '@OUTPUT@'],
  install: true,
  install_dir: 'share/realpaver',
  output: 'rp_params.txt',
)

rp_solver = executable(
  'rp_solver',
  sources: ['rp_solver.cpp', rp_default_params],
  link_with: realpaver_lib,
  include_directories: incdir,
  install: true,
  dependencies: deps_lib,
)

subdir('generators')
