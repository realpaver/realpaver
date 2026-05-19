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

configure_file(
    input: 'configure.hpp.in',
    output: 'configure.hpp',
    configuration: conf_data,
    install: true,
    install_dir: 'include/realpaver',
)

incdir = include_directories(['.', '..'])

rp_sources = []
rp_headers = []

# Interval library
if interval_lib_name.to_upper() == 'GAOL'
    interval_header = custom_target(
        'gen-interval-header',
        input: 'IntervalGaol.hpp',
        output: 'IntervalImpl.hpp',
        command: ['ln', '-f', '@INPUT@', '@OUTPUT@'],
        install: true,
        install_dir: 'include/realpaver',
    )
elif interval_lib_name.to_upper() == 'INUM64'
    interval_header = custom_target(
        'gen-interval-header',
        input: 'IntervalInum64.hpp',
        output: 'IntervalImpl.hpp',
        command: ['ln', '-f', '@INPUT@', '@OUTPUT@'],
        install: true,
        install_dir: 'include/realpaver',
    )
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

lp_header = custom_target(
    'gen-lp-header',
    input: lp_header_hpp,
    output: 'LPSolver.hpp',
    command: ['ln', '-f', '@INPUT@', '@OUTPUT@'],
    install: true,
    install_dir: 'include/realpaver',
)
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

    nlp_header = custom_target(
        'gen-local-solver-header',
        input: local_solver_header_hpp,
        output: 'NLPSolver.hpp',
        command: ['ln', '-f', '@INPUT@', '@OUTPUT@'],
        install: true,
        install_dir: 'include/realpaver',
    )
    rp_sources += [nlp_header]
endif

# flex, bison
if flex.found() and bison.found()
    flex_analyzer = custom_target(
        'flex-parser-gen',
        input: 'parser_flex.lpp',
        output: 'parser_flex.cpp',
        command: [flex, '-o', '@OUTPUT@', '@INPUT@'],
        build_by_default: true,
    )
    rp_sources += [flex_analyzer]

    bison_analyzer = custom_target(
        'bison-parser-gen',
        input: 'parser_bison.ypp',
        output: ['parser_bison.hpp', 'parser_bison.cpp'],
        command: [bison, '-d', '-o', '@OUTPUT1@', '@INPUT@'],
        build_by_default: true,
    )
    rp_sources += [bison_analyzer]
else
    rp_sources += ['parser_flex.cpp', 'parser_bison.cpp']
endif

rp_headers += [
    'AffineCreator.hpp',
    'AffineForm.hpp',
    'Alias.hpp',
    'AssertDebug.hpp',
    'Bitset.hpp',
    'BoxReporter.hpp',
    'Common.hpp',
    'Constraint.hpp',
    'ConstraintArithmetic.hpp',
    'ConstraintCond.hpp',
    'ConstraintFixer.hpp',
    'ConstraintPiecewise.hpp',
    'ConstraintProver.hpp',
    'ConstraintRelation.hpp',
    'ConstraintRep.hpp',
    'ConstraintSet.hpp',
    'ConstraintTable.hpp',
    'ConstraintVisitor.hpp',
    'Contractor.hpp',
    'ContractorACID.hpp',
    'ContractorAffine.hpp',
    'ContractorAffineRevise.hpp',
    'ContractorConstraint.hpp',
    'ContractorDomain.hpp',
    'ContractorFactory.hpp',
    'ContractorHC4.hpp',
    'ContractorHC4Revise.hpp',
    'ContractorList.hpp',
    'ContractorLoop.hpp',
    'ContractorPool.hpp',
    'ContractorPolytope.hpp',
    'ContractorVar3B.hpp',
    'ContractorVar3BCID.hpp',
    'ContractorVarCID.hpp',
    'Dag.hpp',
    'DagExtended.hpp',
    'Domain.hpp',
    'DomainBox.hpp',
    'DomainBoxPicker.hpp',
    'DomainPicker.hpp',
    'DomainSlicer.hpp',
    'DomainSlicerFactory.hpp',
    'DomainSlicerMap.hpp',
    'Double.hpp',
    'DoubleRandom.hpp',
    'Env.hpp',
    'EnvSolver.hpp',
    'EnvOptimizer.hpp',
    'Exception.hpp',
    'FlatFunction.hpp',
    'Inflator.hpp',
    'Integer.hpp',
    'Interval.hpp',
    'IntervalBox.hpp',
    'IntervalBoxPicker.hpp',
    'IntervalFunction.hpp',
    'IntervalFunctionUni.hpp',
    'IntervalFunctionVector.hpp',
    'IntervalGaussSeidel.hpp',
    'IntervalMatrix.hpp',
    'IntervalNewton.hpp',
    'IntervalNewtonParametric.hpp',
    'IntervalNewtonUni.hpp',
    'IntervalPropagator.hpp',
    'IntervalSlicer.hpp',
    'IntervalThickFunction.hpp',
    'IntervalTraits.hpp',
    'IntervalVector.hpp',
    'IntervalUnion.hpp',
    'IntRandom.hpp',
    'Linearizer.hpp',
    'LinearizerAffine.hpp',
    'LinearizerAffineTaylor.hpp',
    'LinearizerTaylor.hpp',
    'Logger.hpp',
    'LPModel.hpp',
    'NLPModel.hpp',
    'Node.hpp',
    'NodeContext.hpp',
    'NodeOptim.hpp',
    'NodePropagator.hpp',
    'NodePropagatorACID.hpp',
    'NodePropagatorAffine.hpp',
    'NodePropagatorHC4.hpp',
    'NodePropagatorList.hpp',
    'NodePropagatorLoop.hpp',
    'NodePropagatorNewton.hpp',
    'NodePropagatorPolytope.hpp',
    'NumericDoubleTraits.hpp',
    'NumericIntervalTraits.hpp',
    'NumericTraits.hpp',
    'NumericMatrix.hpp',
    'NumericVector.hpp',
    'Objective.hpp',
    'Optimizer.hpp',
    'Param.hpp',
    'Parser.hpp',
    'Preprocessor.hpp',
    'Problem.hpp',
    'ProblemBridge.hpp',
    'Prospector.hpp',
    'ProspectorLocalSolver.hpp',
    'ProspectorSimple.hpp',
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
    'Selector.hpp',
    'SelectorLF.hpp',
    'SelectorSF.hpp',
    'SelectorSSR.hpp',
    'Solver.hpp',
    'Space.hpp',
    'SpaceBFS.hpp',
    'SpaceDFS.hpp',
    'SpaceDMDFS.hpp',
    'SpaceHybridDFS.hpp',
    'SpaceOptim.hpp',
    'SpaceOptimLU.hpp',
    'SpaceOptimFD.hpp',
    'Split.hpp',
    'SplitDefault.hpp',
    'SplitRR.hpp',
    'Stat.hpp',
    'SymbolTable.hpp',
    'Term.hpp',
    'TermDeriver.hpp',
    'TermFixer.hpp',
    'TermLin.hpp',
    'TermQuadratic.hpp',
    'Timer.hpp',
    'Tolerance.hpp',
    'UnionFind.hpp',
    'Variable.hpp',
    'VariableVector.hpp',
    'ZeroOne.hpp',
    'IntervalGaol.hpp',
    'LPSolverClp.hpp',
    'LPSolverGurobi.hpp',
]

rp_sources += [
    'AffineCreator.cpp',
    'AffineForm.cpp',
    'Alias.cpp',
    'AssertDebug.hpp',
    'Bitset.cpp',
    'BoxReporter.cpp',
    'Common.cpp',
    'Constraint.cpp',
    'ConstraintArithmetic.cpp',
    'ConstraintCond.cpp',
    'ConstraintFixer.cpp',
    'ConstraintPiecewise.cpp',
    'ConstraintProver.cpp',
    'ConstraintRelation.cpp',
    'ConstraintRep.cpp',
    'ConstraintSet.cpp',
    'ConstraintTable.cpp',
    'ConstraintVisitor.cpp',
    'Contractor.cpp',
    'ContractorACID.cpp',
    'ContractorAffine.cpp',
    'ContractorAffineRevise.cpp',
    'ContractorConstraint.cpp',
    'ContractorDomain.cpp',
    'ContractorFactory.cpp',
    'ContractorHC4.cpp',
    'ContractorHC4Revise.cpp',
    'ContractorList.cpp',
    'ContractorLoop.cpp',
    'ContractorPolytope.cpp',
    'ContractorPool.cpp',
    'ContractorVar3B.cpp',
    'ContractorVar3BCID.cpp',
    'ContractorVarCID.cpp',
    'Dag.cpp',
    'DagExtended.cpp',
    'Domain.cpp',
    'DomainBox.cpp',
    'DomainBoxPicker.cpp',
    'DomainPicker.cpp',
    'DomainSlicer.cpp',
    'DomainSlicerFactory.cpp',
    'DomainSlicerMap.cpp',
    'DoubleRandom.cpp',
    'Env.cpp',
    'EnvOptimizer.cpp',
    'EnvSolver.cpp',
    'Exception.cpp',
    'FlatFunction.cpp',
    'Inflator.cpp',
    'Integer.cpp',
    'Interval.cpp',
    'IntervalBox.cpp',
    'IntervalBoxPicker.cpp',
    'IntervalFunction.cpp',
    'IntervalFunctionUni.cpp',
    'IntervalFunctionVector.cpp',
    'IntervalGaussSeidel.cpp',
    'IntervalMatrix.cpp',
    'IntervalNewton.cpp',
    'IntervalNewtonParametric.cpp',
    'IntervalNewtonUni.cpp',
    'IntervalPropagator.cpp',
    'IntervalSlicer.cpp',
    'IntervalThickFunction.cpp',
    'IntervalVector.cpp',
    'IntervalUnion.cpp',
    'IntRandom.cpp',
    'Linearizer.cpp',
    'LinearizerAffine.cpp',
    'LinearizerAffineTaylor.cpp',
    'LinearizerTaylor.cpp',
    'Logger.cpp',
    'LPModel.cpp',
    'NLPModel.cpp',
    'Node.cpp',
    'NodeContext.cpp',
    'NodeOptim.cpp',
    'NodePropagator.cpp',
    'NodePropagatorACID.cpp',
    'NodePropagatorAffine.cpp',
    'NodePropagatorHC4.cpp',
    'NodePropagatorList.cpp',
    'NodePropagatorLoop.cpp',
    'NodePropagatorNewton.cpp',
    'NodePropagatorPolytope.cpp',
    'Objective.cpp',
    'Optimizer.cpp',
    'Param.cpp',
    'Parser.cpp',
    'Preprocessor.cpp',
    'Problem.cpp',
    'ProblemBridge.cpp',
    'Prospector.cpp',
    'ProspectorLocalSolver.cpp',
    'ProspectorSimple.cpp',
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
    'Selector.cpp',
    'SelectorLF.cpp',
    'SelectorSF.cpp',
    'SelectorSSR.cpp',
    'Solver.cpp',
    'SpaceBFS.cpp',
    'SpaceDFS.cpp',
    'SpaceDMDFS.cpp',
    'SpaceHybridDFS.cpp',
    'SpaceOptim.cpp',
    'SpaceOptimLU.cpp',
    'SpaceOptimFD.cpp',
    'Split.cpp',
    'SplitDefault.cpp',
    'SplitRR.cpp',
    'Stat.cpp',
    'SymbolTable.cpp',
    'Term.cpp',
    'TermDeriver.cpp',
    'TermFixer.cpp',
    'TermLin.cpp',
    'TermQuadratic.cpp',
    'Timer.cpp',
    'Tolerance.cpp',
    'UnionFind.cpp',
    'Variable.cpp',
    'VariableVector.cpp',
    'ZeroOne.cpp',
]

extra_dist = [
    'parser_flex.lpp',
    'parser_bison.ypp',
    'LPSolverClp.cpp',
    'LPSolverClp.hpp',
    'LPSolverGurobi.cpp',
    'LPSolverGurobi.hpp',
    'LPSolverHighs.cpp',
    'LPSolverHighs.hpp',
    'LPSolverSoplex.cpp',
    'LPSolverSoplex.hpp',
    'NLPSolverNlopt.cpp',
    'NLPSolverNlopt.hpp',
]

realpaver_lib = both_libraries(
    'realpaver',
    sources: rp_sources,
    dependencies: deps_lib,
    install: true,
    extra_files: extra_dist,
    include_directories: incdir,
)

install_headers(rp_headers, subdir: 'realpaver')

librealpaver_dep = declare_dependency(include_directories: incdir, link_with: realpaver_lib)
