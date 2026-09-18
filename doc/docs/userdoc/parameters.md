
# Parameters description

RealPaver behavior can be controlled using some parameters. These parameters are defined by the class [`Params`](../api/classrealpaver_1_1_params.md) implemented by the file [`src/realpaver/Param.hpp`](../api/_param_8hpp.md) and [`src/realpaver/Param.cpp`](../api/_param_8cpp.md). Parameters can be handled more globally with environments using the class [`Env`](../api/classrealpaver_1_1_env.md) like for the CSP solver.

Parameters can also be defined in a text file and a default one is provided in `src/realpaver/ParamFile`.

Parameters are (key,value) pairs and three types of values are supported: integer, double and string. String values may be included in a restricted list of words like `YES` or `NO` as well as names of available strategies.

Several categories of parameters can be defined:

- general ones, that mainly apply to global algorithms like the CSP solver, or that are common to most of mechanisms
- specific ones, that are dedicated to some mechanisms like the branching or splitting strategies, the linear solver configuration, etc.

## General parameters

- CERTIFICATION : Postprocessing of solution nodes
	- Domain:
		-  YES: handles clusters and provides certificate of proofs
		-  NO: no certification
	- Default value:  YES
- DISPLAY_REGION : Display of regions
	- Domain:
		-  STD: standard display with one variable and its domain per line
		-  VEC: display as a vector of domains
	- Default value:  STD
- FLOAT_PRECISION : Number of digits used to print floating-point numbers and intervals
	- Domain:  [1..16]
	- Default value:  12
- LOG_LEVEL : Log level
	- Domain:
		-  NONE: no log
		-  MAIN: main level e.g.display results of first
		-  INTER: intermediary level e.g.display nodes in a search algorithm
		-  LOW: low level e.g.display quantities in iterative methods
		-  FULL: verbose mode
	- Default value:  NONE
- OBJ_ABS_TOL : Absolute tolerance on the width of an interval domain of the objective function
	- Domain:  [0, inf]
	- Default value:  0.0001
- OBJ_REL_TOL : Relative tolerance on the width of an interval domain of the objective function
	- Domain:  [0, 1]
	- Default value:  0.0001
- PREPROCESSING : Preprocessing phase of the solving process
	- Domain:
		-  YES: simplification of problems before solving
		-  NO: no simplification
	- Default value:  YES
- TIME_LIMIT : Time limit in seconds
	- Domain:  [0, inf]
	- Default value:  3600
- TRACE : Log level
	- Domain:
		-  YES: display of a trace during a solving process
		-  NO: no trace
	- Default value:  YES
- TRACE_FREQUENCY : Trace displayed every N nodes where N is the value of this parameter
	- Domain:  [1..2147483647]
	- Default value:  1000
- VAR_ABS_TOL : Absolute tolerance on the width of an interval domain of a constrained variable
	- Domain:  [0, inf]
	- Default value:  1e-08
- VAR_REL_TOL : Relative tolerance on the width of an interval domain of a constrained variable
	- Domain:  [0, 1]
	- Default value:  1e-06

## Branch-and-contract algorithms

- BB_INCOMPLETE_SEARCH : Stopping criterion for branch-and-bound algorithms A branch-and-bound algorithm stops when the enclosure of the optimum has the derired precision and at least one feasible point has been found. Now, the search can be stopped as soon as both criteria are satisfied (incomplete search), or it continues until there is no more pending node, which leads to enclose all the minimizers (complete search)
	- Domain:
		-  YES: Incomplete search
		-  NO: Complete search
	- Default value:  YES
- BB_LU_RATIO : In a branch-and-bound algorithm, the strategy Lower-Upper  selects the next node randomly using two criteria, either the lowest lower bound or the lowest upper bound. The criteria 'lowest upper bound' is chosen with probability R where R is the value of this parameter. Typical values lie in [0.2, 0.8].See BB_NODE_SELECTION with value LU
	- Domain:  [0, 1]
	- Default value:  0.5
- BB_NODE_SELECTION : Exploration strategy for branch-and-bound algorithms   lowest lower bound and the lowest upper bound. See the parameter BB_LU_RATIO   to assign the ratio
	- Domain:
		-  LU: Lower
		-  FD: Feasible diving strategy
	- Default value:  FD
- BP_NODE_SELECTION : Exploration strategy for branch-and-prune algorithms   DFS stage is a node whose depth in the search tree is minimal   DFS stage is a node whose perimeter is maximal   a DFS stage is a node whose grid perimeter is maximal Assign SPLIT_INNER_BOX = true for DMDFS and the  hybrid DFS in order to calculate solutions at the given tolerance, and not to consider larger inner boxes as  solutions.
	- Domain:
		-  DFS: Depth
		-  BFS: Breadth
		-  DMDFS: Distant
		-  IDFS: hybrid Best
		-  PDFS: hybrid Best
		-  GPDFS: hybrid Best
	- Default value:  DFS
- DEPTH_LIMIT : Limit on the depth of nodes of search trees
	- Domain:  [1..2147483647]
	- Default value:  1000
- LIMITS_ENABLED : The search is complete when the whole search space is explorated until reaching the tolerances on the variables (and the tolerance on the cost function for optimization problems. On the contrary, the search can be stopped when the following limits are reached: TIME_LIMIT, SOLUTION_LIMIT NODE_LIMIT, and DEPTH_LIMIT.
	- Domain:
		-  YES: the limits are enabled
		-  NO: the limits are disabled
	- Default value:  YES
- NODE_LIMIT : Limit on the number of nodes generated by search algorithms
	- Domain:  [1..2147483647]
	- Default value:  1000000
- SOLUTION_CLUSTER_GAP : Limit on the gap between two solutions such that they are aggregated if they are close enough. The value is negative if no clustering is done; equal to 0.0 if two solutions are aggregated when they overlap; positive otherwise. A huge value may be used to calculate the hull of all the solutions.
	- Domain:  [-1, inf]
	- Default value:  1e-10
- SOLUTION_LIMIT : Limit on the number of solutions
	- Domain:  [1..2147483647]
	- Default value:  1000

## Splitting strategies

- SPLIT_CONTINUOUS : Selection strategy of the next continuous variable in a splitting step, e.g. in the default splitting strategy
	- Domain:
		-  LF: Largest
		-  SSR: Smear
	- Default value:  LF
- SPLIT_DEFAULT_RR_CYCLE : The default splitting strategy is used when we have SPLIT_SELECTION=DEFAULT. It alternates between a cycle of steps based on a round-robin strategy and a cycle of steps based on specific strategies according to the nature of the variables (typically Largest-First for continuous variables and Smallest-First for discrete variables. This parameter controls the length of the round-robin based cycle and it corresponds to a factor of the number of variables of the problem. For example, given 10 variables and the value 1.5, 15 round-robin based steps will be applied consecutively.
	- Domain:  [0, inf]
	- Default value:  3
- SPLIT_DEFAULT_SPECIFIC_CYCLE : The default splitting strategy is used when we have SPLIT_SELECTION=DEFAULT. It alternates between a cycle of steps based on a round-robin strategy and a cycle of steps based on specific strategies according to the nature of the variables (typically Largest-First for continuous variables and Smallest-First for discrete variables. This parameter controls the length of the specific cycle and it corresponds to a factor of the number of variables of the problem. For example, given 10 variables and the value 0.5, 5 specific steps will be applied consecutively.
	- Domain:  [0, inf]
	- Default value:  0.5
- SPLIT_DISCRETE_FIRST : Choice between discrete variables and continuous variables
	- Domain:
		-  YES: discrete variables must be chosen before continuous variables
		-  NO: no impact on the splitting strategy
	- Default value:  NO
- SPLIT_INNER_BOX : An inner box can be considered as a solution or it can be split
	- Domain:
		-  YES: it is split
		-  NO: it is considered as a solution
	- Default value:  NO
- SPLIT_INTERVAL_POINT : An interval [a, b] is split in two parts at a+sip*(b-a) where 0 < sip < 1 denotes this parameter. For example, 0.5 corresponds to the midpoint.
	- Domain:  [0, 1]
	- Default value:  0.45
- SPLIT_OBJECTIVE : The objective function is represented by a variable and its domain can be split or not
	- Domain:
		-  YES: it is considered as any other variable
		-  NO: its domain is not split
	- Default value:  NO
- SPLIT_SELECTION : Variable selection strategy that chooses the next variable to split. RR is a fair round-robin strategy that takes the variables in the ordering given in the model. DEFAULT combines RR with specific strategies applied to continuous and discrete variables
	- Domain:
		-  RR: round
		-  DEFAULT: default strategy
	- Default value:  DEFAULT
- SPLIT_SLICING : Domain splitting strategy
	- Domain:
		-  BI: divides a domain in two parts
	- Default value:  BI

## Contractors and propagation

- LOOP_CONTRACTOR_TOL : Tolerance that corresponds to a percentage of reduction of the width of a box driving the loop contractor.
	- Domain:  [0, 1]
	- Default value:  0.01
- PROPAGATION_BASE : Propagation strategy: the default one is a classical propagation algorithm that uses one contractor per constraint
	- Domain:
		-  HC4: HC4 contractor
		-  AFFINE: contractor based on affine forms
	- Default value:  HC4
- PROPAGATION_TOL : Tolerance that corresponds to a percentage of reduction of the width of a box driving propagation steps. Given two consecutive domains prev and next of some variable and tol the tolerance, a propagation on this variable occurs if we have (1 - width(next) / width(prev)) > tol.
	- Domain:  [0, 1]
	- Default value:  0.001
- PROPAGATION_WITH_ACID : Propagation with a ACID contractor
	- Domain:
		-  YES: with ACID
		-  NO: no ACID
	- Default value:  YES
- PROPAGATION_WITH_NEWTON : Propagation with the interval Newton method applied to a square system
	- Domain:
		-  YES: used
		-  NO: not used
	- Default value:  YES
- PROPAGATION_WITH_POLYTOPE_HULL : Propagation with a polytope hull contractor
	- Domain:
		-  YES: used
		-  NO: not used
	- Default value:  YES
- RELAXATION_EQ_TOL : Positive real number eps used to relax an equation c(x)=0 as |l(x)| <= eps where l(x) is generated by a relaxation method. The value 0.0 implies that a linear equation is generated.
	- Domain:  [0, inf]
	- Default value:  1e-12

## Polytope hull contractor

- AFFINE_APPROXIMATION : Approximation method that generates affine forms for the elementary functions
	- Domain:
		-  MINRANGE: MinRange approximation
		-  CHEBYSHEV: Chebyshev approximation
	- Default value:  CHEBYSHEV
- POLYTOPE_HULL_LOOP : Control of the application of the polytope hull contractor
	- Domain:
		-  YES: it is applied in a fixed
		-  NO: it is applied only once
	- Default value:  NO
- POLYTOPE_HULL_LOOP_TOL : Tolerance that corresponds to a percentage of reduction of the width of a box driving the fixed-point loop
	- Domain:  [0, 1]
	- Default value:  0.1
- POLYTOPE_HULL_RELAXATION : Relaxation method for the polytope hull contractor
	- Domain:
		-  AFFINE: use of affine forms
		-  TAYLOR: use of Taylor (mean value) forms
		-  AFFINE_TAYLOR: use of affine and Taylor (mean value) forms
	- Default value:  AFFINE
- POLYTOPE_HULL_TAYLOR_CORNER : Choice of corners in the Taylor-based relaxation
	- Domain:
		-  RANDOM: one corner selected randomly
		-  RANDOM_OPPOSITE: two opposite corners
		-  one selected randomly
		-  LEFT: corner of left bounds
		-  RIGHT: corner of right bounds
		-  LEFT_RIGHT: corner of right bounds and its opposite corner
	- Default value:  RANDOM_OPPOSITE
- POLYTOPE_HULL_TAYLOR_HANSEN : Use of Hansen's derivatives in the Taylor forms
	- Domain:
		-  YES: Hansen's derivatives
		-  NO: classical derivatives
	- Default value:  NO
- POLYTOPE_HULL_TAYLOR_SEED : Seed used to initialize the generator of pseudo-random numbers. The system is used if it is equal to 0.
	- Domain:  [0..2147483647]
	- Default value:  0

## ACID contractor

- ACID_CT_RATIO : Gain ratio between two consecutive boxes handed by var3BCID contractors in ACID. It is used to adapt the number of var3BCID contractors that are applied in the contraction procedure.
	- Domain:  [0, 1]
	- Default value:  0.002
- ACID_CYCLE_LENGTH : Number of nodes in a learning phase and an exploitation phase of ACID
	- Domain:  [1..2147483647]
	- Default value:  1000
- ACID_HC4_TOL : Tolerance in that corresponds to a percentage of reduction of the width of a box driving propagation steps of the HC4 algorithm used in ACID. Given two consecutive domains prev and next of some variable and tol the tolerance, a propagation on this variable occurs if we have (1-width(next)/width(prev))>tol.
	- Domain:  [0, 1]
	- Default value:  0.1
- ACID_LEARN_LENGTH : Number of consecutive nodes handled in the learning phase of ACID
	- Domain:  [1..2147483647]
	- Default value:  50
- NB_SLICE_3B : Number of slices of a domain handled by var3B contractors
	- Domain:  [1..2147483647]
	- Default value:  7
- NB_SLICE_CID : Number of slices of a domain handled by varCID contractors
	- Domain:  [1..2147483647]
	- Default value:  3
- VAR3BCID_MIN_WIDTH : Threshold on the width of a variable domain used by var3BCID contractors
	- Domain:  [0, inf]
	- Default value:  1e-11

## Local optimization

- NLP_SOLVER_ALGORITHM :   NlOpt
	- Domain:
		-  DEFAULT: default algorithm
		-  NLOPT_NELDERMEAD: derivative
		-  NLOPT_BOBYQA: derivative
		-  NLOPT_MMA: gradient
		-  NLOPT_SLSQP: gradient
		-  NLOPT_LBFGS: gradient
	- Default value:  DEFAULT
- NLP_SOLVER_ITER_LIMIT : Iteration limit
	- Domain:  [1..2147483647]
	- Default value:  100
- NLP_SOLVER_OBJ_ABS_TOL : Absolute tolerance on the objective function
	- Domain:  [0, inf]
	- Default value:  1e-08
- NLP_SOLVER_OBJ_REL_TOL : Relative tolerance on the objective function
	- Domain:  [0, 1]
	- Default value:  0.0001
- NLP_SOLVER_TIME_LIMIT : Time limit in seconds
	- Domain:  [0, inf]
	- Default value:  10
- PROSPECTOR : Exploration strategy to find feasible points
	- Domain:
		-  SIMPLE: simple test of the point returned by the domain picker
		-  LOCALSOLVER: Use a local solver to find a feasible point
		-  INNER_TAYLOR: Use an inner linear relaxation to find a feasible point
		-  INNER_AFFINE: Use an inner linear relaxation to find a feasible point
	- Default value:  INNER_TAYLOR

## Linear solvers

- GAUSSIAN_MIN_PIVOT : Threshold on the values of pivots in Gaussian elimination. A pivot must be strictly greater than this value.
	- Domain:  [0, inf]
	- Default value:  1e-08
- LP_FEAS_TOL : Absolute tolerance on the feasibility of constraints in the LP solver
	- Domain:  [0, inf]
	- Default value:  1e-08
- LP_ITER_LIMIT : Iteration limit of the LP solver
	- Domain:  [1..2147483647]
	- Default value:  100
- LP_TIME_LIMIT : Time limit in seconds of the LP solver
	- Domain:  [0, inf]
	- Default value:  100

## Interval methods

- GAUSS_SEIDEL_ITER_LIMIT : Iteration limit on the number of steps in the interval Gauss-Seidel method
	- Domain:  [1..2147483647]
	- Default value:  50
- GAUSS_SEIDEL_TOL : Tolerance that corresponds to a percentage of reduction  of the width of an interval vector. Given two consecutive domains prev and next of some variable and tol the tolerance, the method is iterated if we have (1-width(next)/width(prev))>tol.
	- Domain:  [0, 1]
	- Default value:  0.001
- INFLATION_CHI : Let x be an interval and let m(x) be its midpoint. An inflation of x returns m(x)+delta*(x-m(x))+chi*[-1,1]. The value of chi must be strictly greater than 0.0, which is necessary to inflate degenerated intervals.
	- Domain:  [0, inf]
	- Default value:  1e-12
- INFLATION_DELTA : Let x be an interval and let m(x) be its midpoint. An inflation of x returns m(x)+delta*(x-m(x))+chi*[-1,1]. The value of delta must be strictly greater than 1.0
	- Domain:  [1, inf]
	- Default value:  1.125
- NEWTON_CERTIFY_ITER_LIMIT : Iteration limit on the number of (outer) steps in the certification technique of the interval Newton method
	- Domain:  [1..2147483647]
	- Default value:  20
- NEWTON_ITER_LIMIT : Iteration limit on the number of (outer) steps in the contraction technique of the interval Newton method
	- Domain:  [1..2147483647]
	- Default value:  30
- NEWTON_TOL : Tolerance that corresponds to a percentage of reduction of the width of a box in the interval Newton method. Given two consecutive domains prev and next of some variable and tol the tolerance, the method is  iterated if we have (1-width(next)/width(prev))>tol.
	- Domain:  [0, 1]
	- Default value:  0.001
- NEWTON_WIDTH_LIMIT : Threshold on the width of an input box of the contraction method of the interval Newton method, which is not applied if the width of the box is strictly greater than this value.
	- Domain:  [0, inf]
	- Default value:  0.5
- UNI_NEWTON_ITER_LIMIT : Iteration limit of the univariate interval Newton method
	- Domain:  [1..2147483647]
	- Default value:  20

## Using the C++ API

Parameters are handled by the [`Params`](../api/classrealpaver_1_1_params.md) class. Below is an example of using this class:

```C++
#include <iostream>
#include "realpaver_api.hpp"

int main()
{
    // Create a Param instance with default values for pre-defined parameters
    Params prm;

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
