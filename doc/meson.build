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

doxygen = find_program('doxygen', required : false)

if not doxygen.found()
  warning('Doxygen not found, no documentation generated!')
else
  cdata = configuration_data()
  cdata.set('REALPAVER_VERSION', meson.project_version())
  cdata.set('TOP_SRCDIR', meson.project_source_root())
  cdata.set('TOP_BUILDDIR', meson.project_build_root())
  if find_program('dot', required : false).found()
    cdata.set('HAVE_DOT', 'YES')
  else
    cdata.set('HAVE_DOT', 'NO')
  endif

  doxyfile = configure_file(input: 'Doxyfile.in',
                            output: 'Doxyfile',
                            configuration: cdata,
                            install: false)

  datadir = join_paths(get_option('datadir'), 'doc', 'realpaver')

  html_target = custom_target('html-realpaver-docs',
    build_by_default: get_option('GENERATE_DOC'),
    input: doxyfile,
    output: 'html',
    command: [doxygen, doxyfile],
    install: true,
    install_dir: datadir)
endif
