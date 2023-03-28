cppunit = dependency('cppunit')

test_bitset = executable('test_bitset',
  sources: 'bitset_test.cpp',
  link_with:realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib+[cppunit]
)
test('Test of bitset', test_bitset)

test_dag = executable('test_dag',
  sources: 'dag_test.cpp',
  link_with:realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib+[cppunit]
)
test('Test of dag', test_dag)

test_hc4revise = executable('test_hc4revise',
  sources: 'hc4revise_test.cpp',
  link_with:realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib+[cppunit]
)
test('Test of hc4revise', test_hc4revise)

test_integer = executable('test_integer',
 sources: 'integer_test.cpp',
 link_with:realpaver_lib,
 include_directories: ['../src']+incdir,
 dependencies: deps_lib+[cppunit]
)
test('Test of integer', test_integer)

test_interval = executable('test_interval',
 sources: 'interval_test.cpp',
 link_with:realpaver_lib,
 include_directories: ['../src']+incdir,
 dependencies: deps_lib+[cppunit]
)
test('Test of interval', test_interval)

test_range = executable('test_range',
 sources: 'range_test.cpp',
 link_with:realpaver_lib,
 include_directories: ['../src']+incdir,
 dependencies: deps_lib+[cppunit]
)
test('Test of range', test_range)

test_real_matrix = executable('test_real_matrix',
 sources: 'real_matrix_test.cpp',
 link_with:realpaver_lib,
 include_directories: ['../src']+incdir,
 dependencies: deps_lib+[cppunit]
)
test('Test of real_matrix', test_real_matrix)

test_scope = executable('test_scope',
  sources: 'scope_test.cpp',
  link_with:realpaver_lib,
  include_directories: ['../src']+incdir,
  dependencies: deps_lib+[cppunit]
)
test('Test of scope', test_scope)
