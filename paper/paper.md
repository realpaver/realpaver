---
title: 'RealPaver 1.1: A C++ Library for Constraint Programming over Numeric or Mixed Discrete-Continuous Domains'
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
date: 5 May 2025
bibliography: paper.bib
---

# Summary

Constraint Programming (CP) is a paradigm for solving constraint satisfaction and optimization problems [@rossi2006]. Although CP mainly addresses combinatorial problems, it can also handle continuous problems by approximating real numbers with intervals [@benhamou2006]. 

RealPaver is a C++ library for CP over numeric or mixed discrete-continuous domains. Constraint Satisfaction Problems (CSPs) can be described either in C++ with the API or in a text file using the syntax of RealPaver specific language.  Then, they can be solved using the C++ API or using the CSP solver from the command line. The CSP solver is pre-configured, but various parameters can be modified in another text file.

With respect to the first version of the software developed twenty years ago [@granvilliers2006], this new library incorporates new types of variables and constraints, new algorithms, a clean object-oriented architecture, the management of parameters, Meson Build as build engine [@meson], an interface with third-party softwares and a C++ API. It achieves performances equivalent to the competing library Ibex [@ibex] for pure continuous problems.

# Statement of need

CP associates a rich modeling language with powerful solving techniques. The main algorithm behind the RealPaver solver is a branch-and-prune (B&P) that implements a complete search to find all the solutions of a given problem [@pvh1997; @chabert2009]. The branching component separates a problem into sub-problems easier to solve. The pruning or contracting component aims at reducing the region delimited by a sub-problem. RealPaver relies on the GAOL interval library [@GAOL] to ensure rigorous computations.

This technology has been applied with success in many fields of engineering like automatic control [@jaulin2001], preliminary design [@yvars2021] and robotics [@merlet2004].

This library can be used by anyone wanting to compute sets of solutions for numerical or mixed discrete-continuous constraint satisfaction problems. It can also be used to prove infeasibility or existence of solutions, thanks to the B\&P algorithm and interval analysis [@moore2009].
Since the library contains most of the state-of-the art algorithms relating to CP over intervals, it can also be used by researchers in this field to define new algorithms.


# Brief overview

## Modeling language

There are three types of variables:

* A boolean variable has domain $\{0, 1\}$;
* An integer variable can take values from a set of integers;
* A real variable lies in a union of intervals.

The language defines several types of constraints:

* An arithmetic constraint involves the usual operations over the reals and relations from the set $\{=, \leq, \geq\}$;
* $G\to B$ is a conditional constraint, where a constraint $B$ (body) is activated when a constraint $G$ (guard) holds true;
* $table(X, S)$ is a table constraint, where $X$ is a vector of variables and $S$ is a set of valid assignments for $X$;
* $piecewise(x, \{I_k\to C_k\}_{k})$ is a piecewise constraint where a constraint $C_k$ is activated when the variable $x$ lies in the interval $I_k$.

## Solving strategies

The B&P algorithm creates a search tree by recursively dividing the initial region, i.e. the Cartesian product of variable domains. Each solving step applies a pruning of domains based on a propagation of contractors:

- the HC4 or BC4 operators [@benhamou1999],
- the ACID algorithm [@neveu2015],
- the interval Newton operator for nonlinear systems of equations [@moore2009],
- linear methods applied to affine or Taylor relaxations of nonlinear problems [@trombettoni2011; @Ninin40R2015]
- specific algorithms for the non-arithmetic or global constraints.

It uses a search strategy responsible for the selection of the next node to explore (depth-first search, breadth-first search and distant-most-depth-first search [@chenouard2009]) and the selection of a variable in this node defining the domain to be split (e.g. largest domains or the greatest impacts on the constraints [@trombettoni2011]), hence generating sub-nodes.


## Parameters and RealPaver customization

RealPaver integrates classes to handle three types of parameters: double-valued, integer-valued or string-valued parameters.

All existing parameters, with their default value, are defined in the class `Params`.
This class organizes them using 10 categories that cover all the aspects of the library.

Moreover, the section about the parameters in the documentation (processed by MkDocs) is generated from the default parameter file, using the Python script `doc/gen_params_doc.py`.


## Building system and requirements

The meson build system is used to orchestrate the configuration, the building of the library, and the generation of `rp_solver` (the CSP solver executable). The user can select one of the supported linear solving libraries (Coin-or CLP, HiGHS, SoPlex and Gurobi) and can activate assertions, logging, or the generation of the documentation, directly as meson command line options.

The current building system does not install dependencies or third party softwares. The user has to install, by its own, the GAOL interval library [@GAOL] and one of the supported linear solving libraries, as well as MkDocs if the building of the documentation is activated.


## Running the solver and getting the solutions

Using the C++ API, one can use the `CSPSolver` class and call the `solve` method after having created a problem instance. Using the command-line executable `rp_solver`, one can solve problems written in a text file following the RealPaver language syntax: 

```bash
rp_solver my_problem.rp -p params.txt
```

The `-p` is optional and allows customizing the parameters using a text file (here `params.txt`). By default, the summary of the solving process and all computed solutions will be stored in a text file, automatically named from the base file name, so `my_problem.sol` in this example. A brief report is also displayed in the console, with the processed files, pre-processing summary, and solving summary (time, number of solutions, solving status, number of nodes in the search tree, and number of pending nodes when ending with a partial solving).


# References
