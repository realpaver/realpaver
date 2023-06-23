
configure_file(input : 'configure_meson.hpp.in',
                output : 'configure.hpp',
                configuration : conf_data,
                install : true,
                install_dir : 'include/realpaver')

incdir = include_directories(['.','..'])

rp_sources = []
rp_headers = []

if interval_lib_name.to_upper() == 'GAOL'
  gaol_header = custom_target('gen-interval-header',
    input: 'IntervalGaol.hpp',
    output: 'IntervalImpl.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir : 'include/realpaver')
  rp_sources += [gaol_header]
elif interval_lib_name.to_upper() == 'INUM64'
  inum64_header = custom_target('gen-interval-header',
    input: 'IntervalInum64.hpp',
    output: 'IntervalImpl.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir : 'include/realpaver')
  rp_sources += [inum64_header]
endif

lp_header_hpp = ''
if lp_lib_name.to_upper() == 'CLP'
  lp_header_hpp += 'LPSolverClp.hpp'
  rp_sources += ['LPSolverClp.cpp']
elif lp_lib_name.to_upper() == 'GUROBI'
#### NOT WORKING FOR NOW 
  lp_header_hpp += 'LPSolverGurobi.hpp'
  rp_sources += ['LPSolverGurobi.cpp']
elif lp_lib_name.to_upper() == 'SOPLEX'
  lp_header_hpp += 'LPSolverSoplex.hpp'
  rp_sources += ['LPSolverSoplex.cpp']
elif lp_lib_name.to_upper() == 'HIGHS'
  lp_header_hpp += 'LPSolverHighs.hpp'
  rp_sources += ['LPSolverHighs.cpp']
endif

lp_header = custom_target('gen-lp-header',
    input: lp_header_hpp,
    output: 'LPSolver.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir : 'include/realpaver')
rp_sources += [lp_header]


if local_solver_name.to_upper() == 'IPOPT'
  rp_headers += ['LocalOptimizerIpopt.hpp']
  rp_sources += ['LocalOptimizerIpopt.cpp']
elif local_solver_name.to_upper() == 'NLOPT'
  rp_headers += ['LocalOptimizerNlopt.hpp']
  rp_sources += ['LocalOptimizerNlopt.cpp']
endif

rp_headers += [
  'AssertDebug.hpp',
  'Bitset.hpp',
  'BC3Contractor.hpp',
  'BC4Contractor.hpp',
  'CIDContractor.hpp',
  'Common.hpp',
  'config.hpp',
  'Constraint.hpp',
  'ConstraintContractor.hpp',
  'ConstraintFixer.hpp',
  'ConstraintRelaxor.hpp',
  'Contractor.hpp',
  'ContractorPool.hpp',
  'Dag.hpp',
  'Double.hpp',
  'Env.hpp',
  'Exception.hpp',
  'ExclusionRegionContractor.hpp',
  'HC4Contractor.hpp',
  'Inflator.hpp',
  'IntContractor.hpp',
  'Integer.hpp',
  'Interval.hpp',
  'IntervalFunction.hpp',
  'IntervalFunctionVector.hpp',
  'IntervalGaussSeidel.hpp',
  'IntervalMatrix.hpp',
  'IntervalNewton.hpp',
  'IntervalRegion.hpp',
  'IntervalSlicer.hpp',
  'IntervalTraits.hpp',
  'IntervalVector.hpp',
  'IntervalUnion.hpp',
  'ListContractor.hpp',
  'LocalOptimizer.hpp',
  'Logger.hpp',
  'LPModel.hpp',
  'MaxCIDContractor.hpp',
  'NcspEnv.hpp',
  'NcspNode.hpp',
  'NcspSpace.hpp',
  'NcspSpaceBFS.hpp',
  'NcspSpaceDFS.hpp',
  'NcspSpaceDMDFS.hpp',
  'NcspSpaceHybridDFS.hpp',
  'NcspSolver.hpp',
  'NcspSplit.hpp',
  'NumericDoubleTraits.hpp',
  'NumericIntervalTraits.hpp',
  'NumericTraits.hpp',
  'NumericMatrix.hpp',
  'NumericVector.hpp',
  'Objective.hpp',
  'Param.hpp',
  'Parser.hpp',
  'parser_bison.hpp',
  'PolytopeHullContractor.hpp',
  'Preprocessor.hpp',
  'Problem.hpp',
  'Propagator.hpp',
  'Prover.hpp',
  'QuadraticTerm.hpp',
  'Range.hpp',
  'RangeSlicer.hpp',
  'RealFunction.hpp',
  'RealFunctionVector.hpp',
  'RealMatrix.hpp',
  'RealPoint.hpp',
  'RealVector.hpp',
  'RLTRelaxation.hpp',
  'Scope.hpp',
  'ScopeMap.hpp',
  'SearchNode.hpp',
  'SplitStrategy.hpp',
  'Stat.hpp',
  'SymbolTable.hpp',
  'Term.hpp',
  'TermDeriver.hpp',
  'TermFixer.hpp',
  'ThickFunction.hpp',
  'Timer.hpp',
  'Tolerance.hpp',
  'UniIntervalNewton.hpp',
  'Variable.hpp',
  'VariableSelector.hpp',
  'VariableVector.hpp',
  'IntervalGaol.hpp',
  'LPSolverClp.hpp',
  'LPSolverGurobi.hpp'
]

rp_sources += [
  'AssertDebug.hpp',
  'Bitset.cpp',
  'BC3Contractor.cpp',
  'BC4Contractor.cpp',
  'CIDContractor.cpp',
  'Common.cpp',
  'Constraint.cpp',
  'ConstraintContractor.cpp',
  'ConstraintFixer.cpp',
  'ConstraintRelaxor.cpp',
  'Contractor.cpp',
  'ContractorPool.cpp',
  'Dag.cpp',
  'Env.cpp',
  'Exception.cpp',
  'ExclusionRegionContractor.cpp',
  'HC4Contractor.cpp',
  'Inflator.cpp',
  'IntContractor.cpp',
  'Integer.cpp',
  'Interval.cpp',
  'IntervalFunction.cpp',
  'IntervalFunctionVector.cpp',
  'IntervalGaussSeidel.cpp',
  'IntervalMatrix.cpp',
  'IntervalNewton.cpp',
  'IntervalRegion.cpp',
  'IntervalSlicer.cpp',
  'IntervalVector.cpp',
  'IntervalUnion.cpp',
  'ListContractor.cpp',
  'LocalOptimizer.cpp',
  'Logger.cpp',
  'LPModel.cpp',
  'MaxCIDContractor.cpp',
  'NcspEnv.cpp',
  'NcspNode.cpp',
  'NcspSpace.cpp',
  'NcspSpaceDFS.cpp',
  'NcspSpaceBFS.cpp',
  'NcspSpaceDMDFS.cpp',
  'NcspSpaceHybridDFS.cpp',
  'NcspSolver.cpp',
  'NcspSplit.cpp',
  'Objective.cpp',
  'Param.cpp',
  'Parser.cpp',
  'parser_flex.cpp',
  'parser_bison.cpp',
  'PolytopeHullContractor.cpp',
  'Preprocessor.cpp',
  'Problem.cpp',
  'Propagator.cpp',
  'Prover.cpp',
  'QuadraticTerm.cpp',
  'Range.cpp',
  'RangeSlicer.cpp',
  'RealFunction.cpp',
  'RealFunctionVector.cpp',
  'RealMatrix.cpp',
  'RealPoint.cpp',
  'RealVector.cpp',
  'RLTRelaxation.cpp',
  'Scope.cpp',
  'ScopeMap.cpp',
  'SearchNode.cpp',
  'Stat.cpp',
  'SymbolTable.cpp',
  'Term.cpp',
  'TermDeriver.cpp',
  'TermFixer.cpp',
  'ThickFunction.cpp',
  'Timer.cpp',
  'Tolerance.cpp',
  'UniIntervalNewton.cpp',
  'Variable.cpp',
  'VariableSelector.cpp',
  'VariableVector.cpp'
]

extra_dist = ['parser_flex.lpp','parser_bison.ypp',
  'LPSolverClp.cpp','LPSolverClp.hpp',
  'LPSolverGurobi.cpp','LPSolverGurobi.hpp',
  'LPSolverHighs.cpp','LPSolverHighs.hpp',
  'LPSolverSoplex.cpp','LPSolverSoplex.hpp']

realpaver_lib = both_libraries('realpaver',
    sources: rp_sources,
    dependencies: deps_lib,
    install: true,
    extra_files: extra_dist,
    include_directories: incdir
)

install_headers(rp_headers, subdir: 'realpaver')
