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
  interval_header = custom_target('gen-interval-header',
    input: 'IntervalGaol.hpp',
    output: 'IntervalImpl.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'include/realpaver')
elif interval_lib_name.to_upper() == 'INUM64'
  interval_header = custom_target('gen-interval-header',
    input: 'IntervalInum64.hpp',
    output: 'IntervalImpl.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'include/realpaver')
else
  error('Invalid interval library: ' + interval_lib_name)
endif
rp_sources += [interval_header]

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
else
  error('Invalid linear solver library: ' + lp_lib_name)
endif

lp_header = custom_target('gen-lp-header',
    input: lp_header_hpp,
    output: 'LPSolver.hpp',
    command: ['ln','-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir : 'include/realpaver')
rp_sources += [lp_header]

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
  'Common.hpp',
  'config.hpp',
  'Constraint.hpp',
  'ConstraintFixer.hpp',
  'ConstraintRelaxor.hpp',
  'Contractor.hpp',
  'ContractorACID.hpp',
  'ContractorBC3Revise.hpp',
  'ContractorBC4Revise.hpp',
  'ContractorBC4.hpp',
  'ContractorConstraint.hpp',
  'ContractorDomain.hpp',
  'ContractorFactory.hpp',
  'ContractorHC4.hpp',
  'ContractorHC4Revise.hpp',
  'ContractorList.hpp',
  'ContractorPool.hpp',
  'ContractorVar3B.hpp',
  'ContractorVar3BCID.hpp',
  'ContractorVarCID.hpp',
  'CSPContext.hpp',
  'CSPEnv.hpp',
  'CSPNode.hpp',
  'CSPPropagator.hpp',
  'CSPSpace.hpp',
  'CSPSpaceBFS.hpp',
  'CSPSpaceDFS.hpp',
  'CSPSpaceDMDFS.hpp',
  'CSPSpaceHybridDFS.hpp',
  'CSPSolver.hpp',
  'CSPSplit.hpp',
  'Dag.hpp',
  'Domain.hpp',
  'DomainBox.hpp',
  'DomainSlicer.hpp',
  'DomainSlicerFactory.hpp',
  'DomainSlicerMap.hpp',
  'Double.hpp',
  'Env.hpp',
  'Exception.hpp',
  'FlatFunction.hpp',
  'Inflator.hpp',
  'Integer.hpp',
  'Interval.hpp',
  'IntervalBox.hpp',
  'IntervalFunction.hpp',
  'IntervalFunctionUni.hpp',
  'IntervalFunctionVector.hpp',
  'IntervalGaussSeidel.hpp',
  'IntervalMatrix.hpp',
  'IntervalNewton.hpp',
  'IntervalNewtonUni.hpp',
  'IntervalPropagator.hpp',
  'IntervalSlicer.hpp',
  'IntervalSmearSumRel.hpp',
  'IntervalThickFunction.hpp',
  'IntervalTraits.hpp',
  'IntervalVector.hpp',
  'IntervalUnion.hpp',
  'Logger.hpp',
  'LPModel.hpp',
  'NLPModel.hpp',
  'NumericDoubleTraits.hpp',
  'NumericIntervalTraits.hpp',
  'NumericTraits.hpp',
  'NumericMatrix.hpp',
  'NumericVector.hpp',
  'Objective.hpp',
  'Param.hpp',
  'Parser.hpp',
  'Preprocessor.hpp',
  'Problem.hpp',
  'Prover.hpp',
  'Range.hpp',
  'RangeSlicer.hpp',
  'RangeUnion.hpp',
  'RealFunction.hpp',
  'RealFunctionVector.hpp',
  'RealMatrix.hpp',
  'RealPoint.hpp',
  'RealVector.hpp',
  'Scope.hpp',
  'ScopeBank.hpp',
  'ScopeMap.hpp',
  'Stat.hpp',
  'SymbolTable.hpp',
  'Term.hpp',
  'TermDeriver.hpp',
  'TermFixer.hpp',
  'TermLin.hpp',
  'TermQuadratic.hpp',
  'Timer.hpp',
  'Tolerance.hpp',
  'Variable.hpp',
  'VariableVector.hpp',
  'ZeroOne.hpp',
  'IntervalGaol.hpp',
  'LPSolverClp.hpp',
  'LPSolverGurobi.hpp'
]

rp_sources += [
  'AssertDebug.hpp',
  'Bitset.cpp',
  'Common.cpp',
  'Constraint.cpp',
  'ConstraintFixer.cpp',
  'ConstraintRelaxor.cpp',
  'Contractor.cpp',
  'ContractorACID.cpp',
  'ContractorBC3Revise.cpp',
  'ContractorBC4Revise.cpp',
  'ContractorBC4.cpp',
  'ContractorConstraint.cpp',
  'ContractorDomain.cpp',
  'ContractorFactory.cpp',
  'ContractorHC4.cpp',
  'ContractorHC4Revise.cpp',
  'ContractorList.cpp',
  'ContractorPool.cpp',
  'ContractorVar3B.cpp',
  'ContractorVar3BCID.cpp',
  'ContractorVarCID.cpp',
  'CSPContext.cpp',
  'CSPEnv.cpp',
  'CSPNode.cpp',
  'CSPPropagator.cpp',
  'CSPSpace.cpp',
  'CSPSpaceBFS.cpp',
  'CSPSpaceDFS.cpp',
  'CSPSpaceDMDFS.cpp',
  'CSPSpaceHybridDFS.cpp',
  'CSPSolver.cpp',
  'CSPSplit.cpp',
  'Dag.cpp',
  'Domain.cpp',
  'DomainBox.cpp',
  'DomainSlicer.cpp',
  'DomainSlicerFactory.cpp',
  'DomainSlicerMap.cpp',
  'Env.cpp',
  'Exception.cpp',
  'FlatFunction.cpp',
  'Inflator.cpp',
  'Integer.cpp',
  'Interval.cpp',
  'IntervalBox.cpp',
  'IntervalFunction.cpp',
  'IntervalFunctionUni.cpp',
  'IntervalFunctionVector.cpp',
  'IntervalGaussSeidel.cpp',
  'IntervalMatrix.cpp',
  'IntervalNewton.cpp',
  'IntervalNewtonUni.cpp',
  'IntervalPropagator.cpp',
  'IntervalSlicer.cpp',
  'IntervalSmearSumRel.cpp',
  'IntervalThickFunction.cpp',
  'IntervalVector.cpp',
  'IntervalUnion.cpp',
  'Logger.cpp',
  'LPModel.cpp',
  'NLPModel.cpp',
  'Objective.cpp',
  'Param.cpp',
  'Parser.cpp',
  'Preprocessor.cpp',
  'Problem.cpp',
  'Prover.cpp',
  'Range.cpp',
  'RangeSlicer.cpp',
  'RangeUnion.cpp',
  'RealFunction.cpp',
  'RealFunctionVector.cpp',
  'RealMatrix.cpp',
  'RealPoint.cpp',
  'RealVector.cpp',
  'Scope.cpp',
  'ScopeBank.cpp',
  'ScopeMap.cpp',
  'Stat.cpp',
  'SymbolTable.cpp',
  'Term.cpp',
  'TermDeriver.cpp',
  'TermFixer.cpp',
  'TermLin.cpp',
  'TermQuadratic.cpp',
  'Timer.cpp',
  'Tolerance.cpp',
  'ZeroOne.cpp',
  'Variable.cpp',
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
