# Parameters description

RealPaver behavior can be controlled using some parameters. These parameters are defined by the class [Param](../api/classrealpaver_1_1_param.md) implemented by the file [`src/realpaver/Param.hpp`](../api/_param_8hpp.md) and [`src/realpaver/Param.cpp`](../api/_param_8cpp.md).

Parameters can also be defined in a text file and a default one is provided in `src/realpaver/ParamFile`.

Parameters are (key,value) pairs and three types of values are supported: integer, double and string. String values may be included in a restricted list of words like `YES` or `NO` as well as names of available strategies.

Several categories of parameters can be defined:

- general ones, that mainly apply to global algorithms like the NCSP solver, or that are common to most of mechanisms
- specific ones, that are dedicated to some mechanisms like the branching or splitting strategies, the linear solver configuration, etc.

## General parameters

- CERTIFICATION: postprocessing of solution nodes in order to handle clusters and to provide certificate of proofs (default: NO)
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
- TIME_LIMIT: time limit used to stop the NCSP solver (default value: 100.0)
- TRACE: display traces in terminal (default value: YES)
- TRACE_FREQUENCY: trace displayed every N nodes where N is the value of this parameter (default: 1000)
- VAR_REL_TOL: relative tolerance on the width of an interval of a constrained variable (default value: 0.0)
- VAR_ABS_TOL: absolute tolerance on the width of an interval of a constrained variable (default value: 1.0e-8)

## Branching parameters

## Splitting parameters

## Contractor and propagation parameters

## Interval methods parameters

## Linear solver parameters

## Local solver parameters

## Using the C++ API
