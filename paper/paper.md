---
title: 'RealPaver: A C++ Library for Constraint Programming over Numeric Domains'
tags:
  - C++
  - constraint programming
  - constraint satisfaction problem
  - nonlinear constraint
  - interval arithmetic
  - modeling language
authors:
  - name: Raphaël Chenouard
    orcid: 0000-0002-9761-6703
    affiliation: 1
    equal-contrib: true
    corresponding: true
  - name: Laurent Granvilliers
    orcid: 0000-0003-3320-7502
    affiliation: 1
    equal-contrib: true
affiliations:
 - name: Nantes Université, École Centrale Nantes, CNRS, LS2N, UMR 6004, Nantes, France
   index: 1
date: 15 January 2025
bibliography: paper.bib
---

# Summary

Constraint Programming (CP) is a paradigm for solving constraint satisfaction and optimization problems [@rossi2006]. Although CP mainly addresses combinatorial problems, it can also handle continuous problems by approximating real numbers with intervals [@benhamou2006]. This technology has been applied with success in many fields of engineering like automatic control [@jaulin2001], preliminary design [@yvars2021] and robotics [@merlet2004].

RealPaver is a C++ library for CP over numeric or mixed discrete-continuous domains. With respect to the first version of the software developed twenty years ago [@granvilliers2006], this new library incorporates new types of variables and constraints, new algorithms, a clean object-oriented architecture, the management of parameters, Meson Build as build engine [@meson], an interface with third-party softwares and a C++ API. It achieves performances equivalent to the competing library Ibex [@chabert2009] for pure continuous problems.

# CP over numeric domains

CP associates a rich modeling language with powerful search techniques. The main algorithm of RealPaver is a branch-and-prune's (B&P) that implements a complete search to find all the solutions of a given problem [@pvh1997; @chabert2009]. The branching component separates a problem into sub-problems easier to solve. The pruning or contracting component aims at reducing the region delimited by a sub-problem.

## Modeling language

There are three types of variables:

* A boolean variable has domain $\{0, 1\}$;
* An integer variable can take values from a set of integers;
* A real variable lies in a union of intervals.

The language defines several types of constraints:

* An arithmetic constraint involves the usual operations over the reals and relations from the set $\{=, \leq, \geq\}$;
* $G\to B$ is a conditional constraint where a constraint $B$ (body) is activated when a constraint $G$ (guard) holds true;
* $table(X, S)$ is a table constraint where $X$ is a vector of variables and $S$ is a set of valid assignments for $X$;
* $piecewise(x, \{I_k\to C_k\}_{k})$ is a piecewise constraint where a constraint $C_k$ is activated when the variable $x$ lies in the interval $I_k$.

## Search strategies

The B&P algorithm creates a search tree by recursively dividing the initial region, i.e. the Cartesian product of variable domains. The search strategy is responsible for the selection of the next node to explore and the selection of a variable in this node. The domain of this variable is then split in several parts, hence generating sub-nodes.

* Node selection. Several strategies are implemented., e.g. depth-first search, breadth-first search and distant-most-depth-first search [@chenouard2009].
* Variable selection. It is possible to select e.g. discrete variables first as done in integer linear programming, discrete variables with the smallest domains, continuous variables with the largest domains or the greatest impacts on the constraints [@trombettoni2011].

## Contraction algorithms

The pruning component of the B&P algorithm combines several techniques.

* Contraint propagation based on HC4 or BC4 operators [benhamou1999] and the ACID algorithm [@neveu2015];
* The interval Newton operator for nonlinear systems of equations [@moore2009];
* Linear methods applied to affine relaxations of nonlinear problems [@trombettoni2011];
* Specific algorithms for the non-arithmetic or global constraints.

# Design and implementation

meson build system

third-party softwares

parameters, threshold and limits to stop complete search

development of new strategies, ..., using the API


# Running Example

mixed-integer, conditional...

# Future work

# Acknowledgements


# References
