lp = executable('lp',
  sources: 'lp.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib
)

if local_solver_dep.found()
  nlp = executable('nlp',
    sources: 'nlp.cpp',
    link_with: realpaver_lib,
    include_directories: ['../src']+incdir,
    dependencies: deps_lib
  )
endif

propag = executable('propag',
  sources: 'propag.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib
)

uni_newton = executable('uni_newton',
  sources: 'uni_newton.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib
)
