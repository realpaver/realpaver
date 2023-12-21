bitset_test = executable( 'bitset_test',
                          sources: 'bitset_test.cpp',
                          link_with: realpaver_lib,
                          include_directories: ['../src'] )
test('Test of bitset', bitset_test)

ctc_3b_test = executable( 'ctc_3b_test',
                        sources: 'ctc_3b_test.cpp',
                        link_with: realpaver_lib,
                        include_directories: ['../src'] )
test('Test of 3B contractor', ctc_3b_test)

ctc_cid_test = executable( 'ctc_cid_test',
                        sources: 'ctc_cid_test.cpp',
                        link_with: realpaver_lib,
                        include_directories: ['../src'] )
test('Test of CID contractor', ctc_cid_test)

ctc_hc4_test = executable( 'ctc_hc4_test',
                       sources: 'ctc_hc4_test.cpp',
                       link_with: realpaver_lib,
                       include_directories: ['../src'] )
test('Test of HC4 contractor', ctc_hc4_test)

dag_test = executable( 'dag_test',
                       sources: 'dag_test.cpp',
                       link_with: realpaver_lib,
                       include_directories: ['../src'] )
test('Test of dag', dag_test)

integer_test = executable( 'integer_test',
                           sources: 'integer_test.cpp',
                           link_with: realpaver_lib,
                           include_directories: ['../src'] )
test('Test of integer', integer_test)

interval_test = executable( 'interval_test',
                            sources: 'interval_test.cpp',
                            link_with: realpaver_lib,
                            include_directories: ['../src'] )
test('Test of interval', interval_test)

range_test = executable( 'range_test',
                         sources: 'range_test.cpp',
                         link_with: realpaver_lib,
                         include_directories: ['../src'] )
test('Test of range', range_test)

real_matrix_test = executable( 'real_matrix_test',
                               sources: 'real_matrix_test.cpp',
                               link_with: realpaver_lib,
                               include_directories: ['../src'] )
test('Test of real matrix', real_matrix_test)

scope_test = executable( 'scope_test',
                         sources: 'scope_test.cpp',
                         link_with: realpaver_lib,
                         include_directories: ['../src'] )
test('Test of scope', scope_test)


realfunctionvector_test = executable( 'realfunctionvector_test',
                         sources: 'realfunctionvector_test.cpp',
                         link_with: realpaver_lib,
                         include_directories: ['../src'] )
test('Test of real function vector', realfunctionvector_test)

if local_solver_dep.found()
  nlpsolver_test = executable( 'nlpsolver_test',
                          sources: 'nlp_solver_test.cpp',
                          link_with: realpaver_lib,
                          dependencies: local_solver_dep,
                          include_directories: ['../src'] )
  test('Test of NLPSolver API', nlpsolver_test)
endif
