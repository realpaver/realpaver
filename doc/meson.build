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
