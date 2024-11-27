---
hide:
  - navigation
  - toc
---

# Welcome to RealPaver documentation

RealPaver allows modeling and solving nonlinear and nonconvex constraint satisfaction and optimization problems over the real numbers. The decision variables, continuous or discrete, have to be bounded. Functions and constraints have to be defined by analytical expressions involving usual arithmetic operations and transcendental elementary functions.

## Rigourousness

RealPaver covers the solution set of a given problem by means of rectangular regions from the real Euclidean space. It can prove the problem insatisfiability by calculating an empty covering. Under some conditions, it can prove the existence of solutions to a set of constraints. Moreover, it is able to enclose the global optimum of an optimization problem with certainty.

## Solving methods

RealPaver implements correctly rounded interval-based computations in a branch-and-bound framework. Its key feature is to combine several methods from various fields: interval fixed-point operators, constraint propagation and local consistency techniques, local optimization using descent methods and metaheuristics, and several search strategies.

## Package

 Realpaver is open source, configurable, object-oriented, and ISO C++ compliant. The API allows the extension of the library along with modeling and solving problems. A mathematical modeling language and a set of benchmarks are also provided. Interval arithmetic is supported by [Gaol](https://github.com/goualard-f/GAOL).
