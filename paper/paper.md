---
title: 'RealPaver 1.1: A C++ Library for Constraint Programming over Numeric Domains'
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

Constraint Programming (CP) is a paradigm for solving constraint satisfaction and optimization problems [@rossi2006]. Although CP mainly addresses combinatorial problems, it can also handle continuous problems by approximating real numbers with intervals [@benhamou2006]. This technology has been applied with success in many fields of engineering like automatic control [@jaulin2001], preliminary design [@yvars2021] and robotics [@merlet2004].

RealPaver is a C++ library for CP over numeric or mixed discrete-continuous domains. With respect to the first version of the software developed twenty years ago [@granvilliers2006], this new library incorporates new types of variables and constraints, new algorithms, a clean object-oriented architecture, the management of parameters, Meson Build as build engine [@meson], an interface with third-party softwares and a C++ API. It achieves performances equivalent to the competing library Ibex [@chabert2009] for pure continuous problems.

# CP over integer and numeric domains

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
* Linear methods applied to affine or Taylor relaxations of nonlinear problems [@trombettoni2011];
* Specific algorithms for the non-arithmetic or global constraints.

# Design and implementation

## Building system and requirements

The meson build system is used to orchestrates the configuration and building of the library and the CSP solver. The user can select one of the supported linear solving libraries (Coin-or CLP, HiGHS, SoPlex and Gurobi) and can activate assertions, logging or the generation of the documentation.

The current building system does not install dependencies or third party softwares. The user has to install by itself the GAOL interval library [@GAOL] and one of the supported linear solving library, as well as MkDocs to generate the documentation.

## Using the CSP solver

Using either the C++ API or the solver binary (called `rp_solver`), the CSP solver is configured by more than 70 parameters that can be described with the C++ API or in a text file.

These parameters are defined by a name, a domain of possible values, a value (string, integer or double), a description and they are organized by categories (general, branch-and-contract, contractors, splitting, etc.). Thus, the user can easily control the configuration or activation of the B\&P internal mechanisms, as well as its global behavior. A parameter file with default values and descriptions is automatically generated during the building of the `rp_solver` binary. Below is an extract of the general parameters section:

```bash
# Time limit in seconds
# Range of possible real values: [0, inf]
TIME_LIMIT = 3600

# Display of regions
# List of possible values: 
# - STD: standard display with one variable and its domain per line
# - VEC: display as a vector of domains
DISPLAY_REGION = STD

# Number of digits used to print floating-point numbers and intervals
# Range of possible int values: [1..16]
FLOAT_PRECISION = 12

# Absolute tolerance on the width of an interval domain of a constrained
# variable
# Range of possible real values: [0, inf]
VAR_ABS_TOL = 1e-08
```

The B\&P behavior can be controlled by defining various parameters, like:

- `SEARCH_STATUS` (`LIMITED` or `COMPLETE`): in the case of limited search, additional parameters act to stop the solving process (`TIME_LIMIT`, `SOLUTION_LIMIT`, `NODE_LIMIT`, `DEPTH_LIMIT`), otherwise they are ignored to compute the whole set of solutions that reaches the tolerances on all the variables.
- `BP_NODE_SELECTION` (`DFS`, `BFS`, `DMDFS`): defines the global exploration strategy


Additionally, more specialized behavior can be configured:

- branching (variable and domain splitting) strategies
- propagation and contraction algorithms (`HC4`, `BC4`, `ACID`, `NEWTON`, etc.)
- parameters dedicated to the selected mechanisms, like the tolerance of the interval Newton contraction algorithm (`NEWTON_TOL`)


## Extension of library

development of new strategies, ..., using the API


# Running Example

The following problem is a simplified sizing problem of an electric coil. Somes variables are continuous (with interval domains), and the other are discrete (an interval of integers or enumerated sets of values). The material of wires is chosen from a table describing its characteristics (density and electical conductivity):

```python
Constants
I = 0.5,    					# current in A
mu_0 = 4*PI*1e-7;

Variables
D in [0.01,0.1],            	# average diameter of coil in m
l in [0.01,0.1],            	# length of coil in m
d in [0.005,0.01],          	# diameter of wire in m
N integer in [10,1000],     	# number of spirals
L in [0.01,0.1],            	# self inductance of coil in H
rho in {2700,8800},  			# material density in kg/m3
gamma in {37.7e6, 59.6e6}; 		# material conductivity in S/m

Aliases
  r_int = 4*N*D/(gamma*d^2),    # coil resistance in Ohm
  phi_w = mu_0*N*PI*D^2*I/(4*l),
  P_j = r_int*I^2,              # Joule losses in W
  M = rho*PI*D*l*d;             # coil mass in kg

Constraints
  L == N*phi_w,
  P_j <= 5,
  M <= 1,
  table({rho,gamma},{
    2700, 37.7e6,
    8800, 59.6e6
  });
```

Solving this problem can be done writting the problem is a text file, for instance called `coil.rp`, then running `rp_solver` on this file from the commande line:

```bash
rp_solver coil.rp -p params.txt
```

The `-p` is optional and allows to customize the parameters using the `params.txt` definition.

An example output is:

```bash
################################################################################
Realpaver 1.1.0 CSP solver
################################################################################
Files
   Input file.......................... ../coil.rp
   Output file......................... coil.sol
################################################################################
Preprocessing
   Time................................ 0.000 (seconds)
   Status.............................. checked
   Number of variables fixed........... 1
   Number of inactive constraints...... 0
################################################################################
Solving
   Time................................ 1.371 (seconds)
   Number of nodes..................... 6301
   Search status....................... partial
   Solution status..................... no proof certificate
   Number of solutions................. 3132
   Time limit enabled.................. 1.000 (seconds)
   Number of pending nodes............. 19
################################################################################
```

Since this problem is under-constrained, a huge number of boxes may be computed to pave the feasible space. In this example, a time limit of 1 second stopped `rp_solver`, leading to a partial exploration with 3,213 solutions and 19 pending boxes (not fully processed boxes).

The whole set of solutions is stored in the text file coil.sol. It also contains the hull of pending boxes and the summary of the solving process and the used values for parameters.

# Future work

# Acknowledgements


# References
