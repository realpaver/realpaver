doxygen = find_program('doxygen', required : false)

if not doxygen.found()
  warning('Doxygen not found, no documentation generated!')
else
  cdata = configuration_data()
  cdata.set('REALPAVER_VERSION', meson.project_version())
  cdata.set('TOP_SRCDIR', meson.source_root())
  cdata.set('TOP_BUILDDIR', meson.build_root())
  if find_program('dot', required : false).found()
    # In the real world this would set the variable
    # to YES. However we set it to NO so that the
    # list of generated files is always the same
    # so tests always pass.
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
    build_by_default: false,
    input: doxyfile,
    output: 'html',
    command: [doxygen, doxyfile],
    install: true,
    install_dir: datadir)
endif
