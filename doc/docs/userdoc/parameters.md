# Parameters description

RealPaver behavior can be controlled using some parameters. These parameters are defined by the class [`Param`](../api/classrealpaver_1_1_param.md) implemented by the file [`src/realpaver/Param.hpp`](../api/_param_8hpp.md) and [`src/realpaver/Param.cpp`](../api/_param_8cpp.md). Parameters can be handled more globally with environments using the class [`Env`](../api/classrealpaver_1_1_env.md) like for the CSP solver.

Parameters can also be defined in a text file and a default one is provided in `src/realpaver/ParamFile`.

Parameters are (key,value) pairs and three types of values are supported: integer, double and string. String values may be included in a restricted list of words like `YES` or `NO` as well as names of available strategies.

Several categories of parameters can be defined:

- general ones, that mainly apply to global algorithms like the CSP solver, or that are common to most of mechanisms
- specific ones, that are dedicated to some mechanisms like the branching or splitting strategies, the linear solver configuration, etc.

## General parameters

- CERTIFICATION: postprocessing of solution nodes in order to handle clusters and to provide certificate of proofs (default: YES)
- DISPLAY_REGION: display of regions (default: STD), possible values:
    - STD: standard display with one variable and its domain per line
    - VEC: display as a vector of domains
- FLOAT_PRECISION: number of digits used to print floating-point numbers and intervals (default value: 8)
- LOG_LEVEL: level for logging (default: NONE), possible values:
    - NONE: no logging
    - MAIN:  main level e.g. display results of first-class algorithms
    - INTER: intermediary level e.g. display nodes in a search algorithm
    - LOW:   low level e.g. display quantities in iterative methods
    - FULL:  verbose mode
- PREPROCESSING: preprocessing phase of the solving process enabled or disabled (default: YES)
- TIME_LIMIT: time limit used to stop the CSP solver (default value: 100.0)
- TRACE: display traces in terminal (default value: YES)
- TRACE_FREQUENCY: trace displayed every N nodes where N is the value of this parameter (default: 1000)
- VAR_REL_TOL: relative tolerance on the width of an interval of a constrained variable (default value: 0.0)
- VAR_ABS_TOL: absolute tolerance on the width of an interval of a constrained variable (default value: 1.0e-8)

## Branching parameters

- BB_SPACE_FREQUENCY: in a branch-and-bound algorithm, the next node can be either the node with the lowest lower bound or the node with the lowest upper bound (diving). The frequency manages the selection of the next node (default value: 1), possible values:
    - 1 means that the node with the lowest lower bound is always selected;
    - a value > 1 means that the node with the lowest upper bound is selected when the node counter modulo the frequency is equal to 0.
- BP_NODE_SELECTION: exploration strategy for branch-and-prune algorithms (default value: DFS), possible values:
    - DFS: Depth-First-Search
    - BFS: Breadth-First-Search
    - DMDFS: Distant-Most Depth-First-Search
    - IDFS: Hybrid Best-First Depth-First strategy such that the next node after a DFS stage is a node whose depth in the search tree is minimal
    - PDFS: Hybrid Best-First Depth-First strategy such that the next node after a DFS stage is a node whose perimeter is maximal
    - GPDFS: Hybrid Best-First Depth-First strategy such that the next node after a DFS stage is a node whose grid perimeter is maximal
- DEPTH_LIMIT: limit on the depth of nodes of search trees (default value: 100)
- NODE_LIMIT: limit on the number of nodes generated by search algorithms (default value: 100000)
- SOLUTION_LIMIT: limit on the number of solutions (default value: 100)
- SOLUTION_CLUSTER_GAP: limit on the gap between two solutions such that they are aggregated if they are close enough (default value: 0.0), possible values:
    - negative if no clustering is required;
    - 0.0 if two solutions are aggregated if they overlap;
    - positive otherwise; in particular, a huge value may permit to calculate the hull of all the solutions.

## Splitting parameters

- SPLIT_INNER_BOX: enforce the width of inner boxes to be lower than the tolerance of each dimension, otherwise they are considered as solutions (default value: NO)
- SPLIT_INTERVAL_POINT: an interval [a, b] is split in two parts at a+sip*(b-a) where 0 < sip < 1 denotes this parameter. For example, 0.5 corresponds to the midpoint. (default value: 0.45)
- SPLIT_OBJECTIVE: the objective function is represented by a variable and its domain can be split or not (default value: NO)
- SPLIT_SELECTION: variable selection strategy that chooses the next variable to split (default value: RR), possible values:
    - RR:  round-robin strategy
    - LF:  largest domain
    - SF:  smallest domain
    - SSR: smear sum relative (using derivatives)
    - SLF: integer variable with smallest first and then real variable with largest domain
- SPLIT_SLICING: domain splitting strategy (default value: BI), possible values:
    - BI: divides a domain in two parts

## Contractor and propagation parameters

- ACID_CT_RATIO: gain ratio between two consecutive boxes handed by var3BCID contractors in ACID, (default value: 0.002). It is used to adapt the number of var3BCID contractors that are applied in the contraction procedure.
- ACID_CYCLE_LENGTH: number of nodes in a learning phase and an exploitation phase of ACID (default value: 1000)
- ACID_HC4_REL_TOL: tolerance in [0,1] that corresponds to a percentage of reduction of the width of a box driving propagation steps of the HC4 algorithm used in ACID (default value: 1.0e-1). Given two consecutive domains prev and next of some variable and tol the tolerance, a propagation on this variable occurs if we have (1 - width(next) / width(prev)) > tol.
- ACID_LEARN_LENGTH: number of consecutive nodes handled in the learning phase of ACID (default value: 50)
- BC3_PEEL_FACTOR: peel factor for the BC3 contractor >= 0.0 and <= 100.0 (default value: 2.0). Given an interval [a, b] and the peel factor f, let w = (b - a) * (p / 100) be a ratio of the interval width. Then the consistency of the intervals [a, a+w] and [b-w, w] is checked in order to stop the search.
- BC3_ITER_LIMIT: iteration limit for the iterative method of the BC3 contractor used to find the outermost zeros of a function in a given interval (default value: 30)
- NB_SLICE_3B: number of slices of a domain handled by var3B contractors (default value: 7)
- NB_SLICE_CID: number of slices of a domain handled by varCID contractors (default value: 3)
- PROPAGATION_BASE: Propagation strategy (default value: HC4), possible values:
    - HC4:   HC4 contractor
    - BC4:   BC4 contractor
    - ACID : ACID contractor
- PROPAGATION_REL_TOL: tolerance in [0,1] that corresponds to a percentage of reduction of the width of a box driving propagation steps (default value: 1.0e-2). Given two consecutive domains prev and next of some variable and tol the tolerance, a propagation on this variable occurs if (1 - width(next) / width(prev)) > tol.
- PROPAGATION_WITH_NEWTON: propagation with the interval Newton method applied to a square system of equations (default value: NO)
- RELAXATION_EQ_TOL: positive real number eps used to relax an equation c(x)=0 as |l(x)| <= eps, where l(x) is generated by a relaxation method (default value: 1.0e-10). The value 0.0 implies that a linear equation is generated.
- VAR3BCID_MIN_WIDTH: threshold on the width of a variable domain used by var3BCID contractors (default value: 1.0e-11)

## Interval methods parameters

- GAUSS_SEIDEL_ITER_LIMIT: iteration limit on the number of steps in the interval Gauss-Seidel method (default value: 50)
- GAUSS_SEIDEL_REL_TOL: tolerance in [0,1] that corresponds to a percentage of reduction of the width of an interval vector (default value: 1.0e-3). Given two consecutive domains prev and next of some variable and tol the tolerance, the method is iterated if we have (1 - width(next) / width(prev)) > tol.
- INFLATION_DELTA: let x be an interval and let m(x) be its midpoint, an inflation of x returns m(x) + delta*(x - m(x)) + chi*[-1,1] (default value: 1.125). This parameter must be greater than 1.0.
- INFLATION_CHI: let x be an interval and let m(x) be its midpoint, an inflation of x returns m(x) + delta*(x - m(x)) + chi*[-1,1] (default value: 1.0e-12). This parameter is a small real number > 0.0 which is necessary to inflate degenerated intervals.
- NEWTON_CERTIFY_ITER_LIMIT: iteration limit on the number of (outer) steps in the certification technique of the interval Newton method (default value: 20)
- NEWTON_ITER_LIMIT: iteration limit on the number of (outer) steps in the contraction technique of interval Newton method (default value: 30)
- NEWTON_REL_TOL: tolerance in [0,1] that corresponds to a percentage of reduction of the width of a box (default value: 1.0e-3). Given two consecutive domains prev and next of some variable and tol the tolerance, the method is iterated if we have (1 - width(next) / width(prev)) > tol.
- NEWTON_WIDTH_LIMIT: threshold on the width of an input box of the contraction method, whih is not applied if the width of the box is greater than this value (default value: 0.5)
- UNI_NEWTON_ITER_LIMIT: iteration limit for the univariate interval Newton method (default value: 20)

## Linear solver parameters

- LP_TIME_LIMIT: time limit in seconds (default value: 10.0)
- LP_ITER_LIMIT: iteration limit (default value: 50)
- GAUSSIAN_MIN_PIVOT: threshold on the values of pivots in Gaussian elimination (defautl value: 1.0e-8). A pivot must be strictly greater than this (positive) value.

## Local solver parameters

- NLP_SOLVER_ALGORITHM: algorithm implemented by the NLP solver (default value: DEFAULT), possible values:
    - DEFAULT: the default algorithm (NLOPT_SLSQP for constrained problems and NLOPT_NELDERMEAD otherwise)
    - NLOPT_NELDERMEAD: derivative-free unconstrained local optimization using NlOpt
    - NLOPT_BOBYQA: derivative-free unconstrained local optimization using NlOpt
    - NLOPT_MMA: gradient-based constrained local optimization using NlOpt
    - NLOPT_SLSQP: gradient-based constrained local optimization using NlOpt
- NLP_SOLVER_TIME_LIMIT: time limit in seconds (default value: 10.0)
- NLP_SOLVER_ITER_LIMIT: iteration limit (default value: 100)
- NLP_SOLVER_OBJ_REL_TOL: relative tolerance on the objective function (default value: 1.0e-4)
- NLP_SOLVER_OBJ_ABS_TOL: absolute tolerance on the objective function (default value: 0.0)


## Using the C++ API

Parameters are handled by the [`Param`](../api/classrealpaver_1_1_param.md) class. Below is an example of using this class:

```C++
#include <iostream>
#include "realpaver_api.hpp"

int main()
{
    // Create a Param instance with default values for pre-defined parameters
    Param prm;

    // Print the list of defined parameters with their values
    std::cout<<prm<<std::endl;

    // Change the integer parameter relating to the maximal depth of the search tree
    prm.setIntParam("DEPTH_LIMIT", 1000);
    // Change the double parameter relating to the time limit
    prm.setDblParam("TIME_LIMIT", 3.6e3);
    // Change the string parameter relating to the logging level
    prm.setStrParam("LOG_LEVEL", "MAIN");

    // Printing current value for double parameter relating to relative tolerance
    std::cout<<"Current relative tolerance: "<<prm.getDblParam("VAR_REL_TOL")<<std::endl;

    // Load parameters from a file
    prm.loadParam("my_params.txt");

    return 0;
}
```
