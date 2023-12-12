configure_file(input: 'configure_meson.hpp.in',
                output: 'configure.hpp',
                configuration: conf_data,
                install: true,
                install_dir: 'include/realpaver')

incdir = include_directories(['.','..'])

rp_sources = []
rp_headers = []

# Interval library
if interval_lib_name.to_upper() == 'GAOL'
  gaol_header = custom_target('gen-interval-header',
    input: 'IntervalGaol.hpp',
    output: 'IntervalImpl.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'include/realpaver')
elif interval_lib_name.to_upper() == 'INUM64'
  inum64_header = custom_target('gen-interval-header',
    input: 'IntervalInum64.hpp',
    output: 'IntervalImpl.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'include/realpaver')
endif

# LP library
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

# Local NLP solver
if local_solver_dep.found()
  local_solver_header_hpp = ''
  if local_solver_name.to_upper() == 'IPOPT'
    local_solver_header_hpp = ['NLPSolverIpopt.hpp']
    rp_sources += ['NLPSolverIpopt.cpp']
  elif local_solver_name.to_upper() == 'NLOPT'
    local_solver_header_hpp = ['NLPSolverNlopt.hpp']
    rp_sources += ['NLPSolverNlopt.cpp']
  endif

  nlp_header = custom_target('gen-local-solver-header',
    input: local_solver_header_hpp,
    output: 'NLPSolver.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'include/realpaver')
rp_sources += [nlp_header]
endif

# flex, bison
if flex.found() and bison.found()
   flex_analyzer = custom_target('flex-parser-gen',
      input: 'parser_flex.lpp',
      output: 'parser_flex.cpp',
      command: [flex, '-o', '@OUTPUT@', '@INPUT@'])
   rp_sources += [flex_analyzer]

   bison_analyzer = custom_target('bison-parser-gen',
      input: 'parser_bison.ypp',
      output: ['parser_bison.hpp', 'parser_bison.cpp'],
      command: [bison, '-d', '-o', '@OUTPUT1@', '@INPUT@'])
   rp_sources += [bison_analyzer]
else
   rp_sources += ['parser_flex.cpp',
                  'parser_bison.cpp']
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
  'Domain.hpp',
  'DomainContractor.hpp',
  'DomainBox.hpp',
  'DomainSlicer.hpp',
  'DomainSlicerFactory.hpp',
  'Double.hpp',
  'Env.hpp',
  'Exception.hpp',
  'HC4Contractor.hpp',
  'Inflator.hpp',
  'Integer.hpp',
  'Interval.hpp',
  'IntervalBox.hpp',
  'IntervalFunction.hpp',
  'IntervalFunctionVector.hpp',
  'IntervalGaussSeidel.hpp',
  'IntervalMatrix.hpp',
  'IntervalNewton.hpp',
  'IntervalSlicer.hpp',
  'IntervalTraits.hpp',
  'IntervalVector.hpp',
  'IntervalUnion.hpp',
  'ListContractor.hpp',
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
  'NLPModel.hpp',
  'NumericDoubleTraits.hpp',
  'NumericIntervalTraits.hpp',
  'NumericTraits.hpp',
  'NumericMatrix.hpp',
  'NumericVector.hpp',
  'Objective.hpp',
  'Param.hpp',
  'Parser.hpp',
  'PolytopeHullContractor.hpp',
  'Preprocessor.hpp',
  'Problem.hpp',
  'Propagator.hpp',
  'Prover.hpp',
  'QuadraticTerm.hpp',
  'Range.hpp',
  'RangeSlicer.hpp',
  'RangeUnion.hpp',
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
  'ZeroOne.hpp',
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
  'Domain.cpp',
  'DomainContractor.cpp',
  'DomainBox.cpp',
  'DomainSlicer.cpp',
  'DomainSlicerFactory.cpp',
  'Env.cpp',
  'Exception.cpp',
  'HC4Contractor.cpp',
  'Inflator.cpp',
  'Integer.cpp',
  'Interval.cpp',
  'IntervalBox.cpp',
  'IntervalFunction.cpp',
  'IntervalFunctionVector.cpp',
  'IntervalGaussSeidel.cpp',
  'IntervalMatrix.cpp',
  'IntervalNewton.cpp',
  'IntervalSlicer.cpp',
  'IntervalVector.cpp',
  'IntervalUnion.cpp',
  'ListContractor.cpp',
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
  'NLPModel.cpp',
  'Objective.cpp',
  'Param.cpp',
  'Parser.cpp',
  'PolytopeHullContractor.cpp',
  'Preprocessor.cpp',
  'Problem.cpp',
  'Propagator.cpp',
  'Prover.cpp',
  'QuadraticTerm.cpp',
  'Range.cpp',
  'RangeSlicer.cpp',
  'RangeUnion.cpp',
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
  'ZeroOne.cpp',
  'Variable.cpp',
  'VariableSelector.cpp',
  'VariableVector.cpp'
]

extra_dist = ['parser_flex.lpp','parser_bison.ypp',
  'LPSolverClp.cpp','LPSolverClp.hpp',
  'LPSolverGurobi.cpp','LPSolverGurobi.hpp',
  'LPSolverHighs.cpp','LPSolverHighs.hpp',
  'LPSolverSoplex.cpp','LPSolverSoplex.hpp',
  'NLPSolverIpopt.cpp','NLPSolverIpopt.hpp',
  'NLPSolverNlopt.cpp','NLPSolverNlopt.hpp']

realpaver_lib = both_libraries('realpaver',
    sources: rp_sources,
    dependencies: deps_lib,
    install: true,
    extra_files: extra_dist,
    include_directories: incdir
)

install_headers(rp_headers, subdir: 'realpaver')
