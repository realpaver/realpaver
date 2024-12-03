---
hide:
  - navigation
---


# Welcome to RealPaver documentation

RealPaver allows modeling and solving nonlinear constraint satisfaction and optimization problems involving continuous or discrete variables associated with domains. Functions and constraints are defined by analytical expressions.

The current stable version addresses only constraint satisfaction problems.

## Rigourousness

RealPaver calculates a _paving_ or covering of the solution space of a given problem by means of interval boxes. It is able to provide different types of proof certificates related to the satisfiability of a set of constraints.

## Solving methods

RealPaver implements correctly rounded interval-based computations in a branch-and-prune framework. Its key feature is to combine several methods from various fields: interval fixed-point operators, constraint propagation and local consistency techniques, local optimization using descent methods and metaheuristics, and several search strategies.

## Package

RealPaver is open source, configurable, object-oriented, and ISO C++ compliant. The API allows the extension of the library along with modeling and solving problems. A mathematical modeling language and a set of benchmarks are also provided. Interval arithmetic is supported by [Gaol](https://github.com/goualard-f/GAOL).
