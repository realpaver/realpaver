dev = executable('dev',
  sources: 'dev.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib
)

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

parsing = executable('parsing',
  sources: 'parsing.cpp',
  link_with: realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib
)

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

# Sub-directories
subdir('ncsp')
