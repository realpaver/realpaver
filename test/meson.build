bitset_test = executable( 'bitset_test',
                          sources: 'bitset_test.cpp',
                          link_with:realpaver_lib,
                          include_directories: ['../src'] )
test('Test of bitset', bitset_test)

hc4_test = executable( 'hc4_test',
                       sources: 'hc4_test.cpp',
                       link_with:realpaver_lib,
                       include_directories: ['../src'] )
test('Test of HC4', hc4_test)

dag_test = executable( 'dag_test',
                       sources: 'dag_test.cpp',
                       link_with:realpaver_lib,
                       include_directories: ['../src'] )
test('Test of dag', dag_test)

integer_test = executable( 'integer_test',
                           sources: 'integer_test.cpp',
                           link_with:realpaver_lib,
                           include_directories: ['../src'] )
test('Test of integer', integer_test)

interval_test = executable( 'interval_test',
                            sources: 'interval_test.cpp',
                            link_with:realpaver_lib,
                            include_directories: ['../src'] )
test('Test of interval', interval_test)

range_test = executable( 'range_test',
                         sources: 'range_test.cpp',
                         link_with:realpaver_lib,
                         include_directories: ['../src'] )
test('Test of range', range_test)

real_matrix_test = executable( 'real_matrix_test',
                               sources: 'real_matrix_test.cpp',
                               link_with:realpaver_lib,
                               include_directories: ['../src'] )
test('Test of real matrix', real_matrix_test)

scope_test = executable( 'scope_test',
                         sources: 'scope_test.cpp',
                         link_with:realpaver_lib,
                         include_directories: ['../src'] )
test('Test of scope', scope_test)


realfunctionvector_test = executable( 'realfunctionvector_test',
                         sources: 'realfunctionvector_test.cpp',
                         link_with:realpaver_lib,
                         include_directories: ['../src'] )
test('Test of realfunctionvector', realfunctionvector_test)


# exclusion_contractor_test = executable( 'exclusion_contractor_test',
#                          sources: 'exclusion_contractor_test.cpp',
#                          link_with:realpaver_lib,
#                          include_directories: ['../src'] )
# test('Test of ExclusionContractor', exclusion_contractor_test)

# if local_solver_name.to_lower() =='nlopt'
#     nlopt_test = executable( 'nlopt_test',
#                             sources: 'nlopt_test.cpp',
#                             link_with:realpaver_lib,
#                             dependencies: local_solver_dep,
#                             include_directories: ['../src'] )
#     test('Test of Nlopt API', nlopt_test)
# elif local_solver_name.to_lower() =='ipopt'
#     ipopt_test = executable( 'ipopt_test',
#                             sources: 'ipopt_test.cpp',
#                             link_with:realpaver_lib,
#                             dependencies: local_solver_dep,
#                             include_directories: ['../src'] )
#     test('Test of Ipopt API', ipopt_test)
# endif