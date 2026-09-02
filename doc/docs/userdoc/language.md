# Modeling language

The RealPaver library provides a parser for the dedicated RealPaver modeling language
to state *Constraint Satisfaction Problems* (CSPs) or *Constrained Optimization Problems* (COPs).
This section describes this dedicated language that can be used to define files parsed as `Problem`
in any C++ code or directly as input for the CSP solver binary.

The language supports 6 different statements at the global scope:

- `Constants` definitions
- `Variables` definitions
- `Aliases` definitions
- `Functions` definitions
- `Constraints` definitions
- `Objectives` definitions

These 6 kinds of statements allow to define a **list** of each type of elements.
Namely, the `Constants` statement defines a list of constants, all separated by a `,`and terminated by a `;`, even in the case of a single constant definition.

These statements can be used several times in the same file.

## Comments

Single line comments can be defined using the `#` character:

``` py
# This is a comment
```

## Expressions and pre-defined functions

In RealPaver, most common arithmetic expressions are supported over the real numbers.
The common unary (`+,-`) and binary operators can be used (`+,-,/,*,^`).

Additionally, the most common mathematical functions are pre-defined in the langage:
`abs, sqr, sqrt, pow, exp, log, cos, sin, tan, cosh, sinh, tanh`.

`^` and `pow` can be used similarly as `x^y`or `pow(x,y)`, where $y$ can be an integer or a real number.

Enclosing `|` around an expression is similar to using the `abs` function: `|x|`is equivalent to `abs(x)`.

Additional user-functions can be defined (see [functions definition](#functions)) and called like any other functions.

## Constants

Defining constants starts with the `Constants` keyword followed by the list of constants definition as names
with assigned value. The generic syntax is:

``` py
Constants
  name_1 = expr_1,
  ...,
  name_n = expr_n;
```

**NB:** The expression used to defined a constant can use a previously defined constant and any constant arithmetic expression is valid.

A few constants are already defined and shouldn't be redefined as:
`PI`, `-inf` (for $-\infty$) and  `+inf` (for $\infty$).

Here are 2 examples with a single constant definition and a list of 2 constants:

``` py
Constants
  g = 9.81;
```

``` py
Constants
  R = 8,31446261815324,   # Ideal gas constant in J.K^−1.mol^-1
  k_B = 1.380649*1e-23;   # Boltzmann constant in J.K^−1
```

## Variables

The definition of a variable is a bit more complex, since a domain should be defined, as well as the width of
interval values considered as solutions during a solving process (for continuous variables). This width is defined as a tolerance with
a relative and absolute value.

Additionally to a domain, a variable can be typed using keywords: `binary`, `integer`, `real`.
It can be used to infer the domain when it is not obvious (e.g. interval of continuous integers).
When defining a binary variable, the domain $[0,1]$ must not be specified.

The generic syntax to define a variable is:

```
var_name type in a_domain
```

For continuous variables the main syntax is:

```
var_name in a_domain tol(rel_tol,abs_tol)
```

To define domains, interval values are defined by 2 values between square brackets: `[-1,1]`,
whereas enumerated set values are defined with brackets: `{1,4,10}`.

Like for constants, variables can only be defined as list of variables with the keyword `Variables`.
Here are two examples with the definition of a single variable and the definition of a list of 3 variables:

``` py
Variables
  a in [-inf,+inf];
```

The variable $a$ in defined over the interval $[\-infty,\infty]$ with default tolerances.

``` py
Variables
  x in [-PI,2*PI/3] tol(0.,1e-3),
  y integer in {0,1,2,4,8},
  z binary;
```

The variable $x$ is a real variable defined on the interval $[-\pi, 2\pi/3]$,
with a relative tolerance equal to 0 and an absolute tolerance of 0.001.
The variable $y$ is a discrete variable with its domain defined by the set of integers $\{0,1,2,4,8\}$
and $z$ is a binary variable.

## Aliases

They can be used as a shortcut to represent an expression (composed of constants as well as variables).

The generic syntax is:

```
alias_name = expression
```

Aliases are defined in list, with the `Aliases` keyword.
Here are two examples:

``` py
Aliases
  cos2x = cos(x)^2;
```

The alias $cos2x$ is a shortcut for the expression $cos(x)^2$.
Here, we make the assumption that variable $x$ is previously defined.

``` py
Variables
  radius in [1,5];
Aliases
  perimeter = 2*PI*radius,
  area = PI*radius^2;
```


## Functions

In many cases aliases may not be enough to defined shortcut to expressions,
more especially when they are parameterized by at least one parameter.
The generic syntax for function is:

```
fun_name ( parameters ) = expression
```

The parameters are symbol names separated by `,` and the expression should include them. Conversely,
all symbols, not being defined as constants, must be listed as parameters.

Like other statements, functions are defined as lists, using the keyword `Functions`.

Here are two examples:

``` py
Constants
  R = 8,31446261815324;
Functions
  volume(p,n,R) = n*R*T/p
```

In this example, the volume is a function of the pressure $p$, the number of moles $n$, the temperature ($T$)
and the ideal gas constant $R$.

``` py
Functions
  monomial(a,x,d) = a*x^d,
  poly3(a,b,c,d,x) = monomial(a,x,3) + monomial(b,x,2) + c*x + d;
```

In this second example, poly3 calls the function monomial to build the symbolic expression
of a polynomial of degree 3.


## Constraints

Like other statements at the global level, List of constraints must be defined even for a single one.
Then, the `Constraints`keyword has to be used to start the list of constraints.
Several types of constraints can be defined:

1. Arithmetic constraints based on $\leq$, $==$ or $\geq$ comparison operators:
2. Set constraints based on the $in$ operator, with the right operand restricted to an interval:
`
expression in [a,b]
`
3. Conditional constraints, with the condition being defined as a constraint and an imply operator `->`:
`
constraint -> constraint
`
4. Table constraints, enumerating all consistent tuples for a set of variables:
`
table(var_set,tuple_set)
`

5. Piecewise constraints, conditioned by a variable domain to activate the single constraint to apply:
`
piecewise(x,{[a_1,b_1]:constraint_1, ..., [a_n,b_n]:constraint_n})
`


**NB:**

- Only conditions as set constraints are supported by the solver
- Intervals in a piecewise constraint must not overlap (except on their bounds)

Here are some example constraints:

``` py
Constraints
  x^2 + y^2 <= r^2,
  x + 2*y in [1,2],
  z <=0 -> z^2 >= 2,
  table({x,y},{
    1,   1,
    1.5, 0.4,
    2.1, 1.6
  });
```

## Objectives

One or several objectives can be defined using the `Objectives` keyword.
An objective is defined by a direction (`MIN` or `MAX`) and an arithmetic expression.

For instance minimizing the Sphere function over 3 dimensions can be expressed as:

``` py
Variables
  x1 in [-inf,+inf],
  x2 in [-inf,+inf],
  x3 in [-inf,+inf];
Objectives
  MIN x1^2 + x2^2 + x3^2;
```

The Schaffer bi-objective optimization problem is simply defined as:

``` py
Variables
  x in [-10,10];
Objectives
  MIN x^2,
  MIN (x-2)^2;
```

**NB:** Currently, no optimization algorithm is implemented!


## A complete example

``` py
# Coil compression spring design problem, finds minimum volume
# of wire for the production of a coil compression spring.
#
# Sangren, E, Nonlinear Integer and Discrete Programming in Mechanical
# Design Optimization. Journal of Mechanical Design, Trans.
# ASME 112 (1990), 223-229.

Constants
   Pload = 300,   # preload                   (lb)
   Pmax = 1000,   # max working load          (lb)
   delm = 6,      # max deflection            (in)
   delw = 1.25,   # deflection from preload   (in)
   lmax = 14,     # max free length of spring (in)
   coilmax = 3,   # max coil diameter         (in)
   S = 189e3,     # max shear stress
   G =11.5e6,     # shear modulus of material

   del_up = delm/Pload,
   del_lo = delw/(Pmax - Pload);

Variables
   coil in [0,1e8],         # coil diameter
   wire in [0.2,0.5],       # wire diameter
   del in [del_lo, del_up], # deflection
   N integer in [0,1e3],    # number of coils
   C in [1.1,1e8],
   K in [0,1e8],
   volume in [0,1e8];

Objectives
  MIN volume;

Constraints
    volume == ((N + 2)/2)*PI*coil*wire^2, # definition of total volume of coil spring
    C == coil/wire,
    K == (4*C - 1)/(4*C - 4) + 0.615/C,
    8*(Pmax/PI)*K*(C/(wire^2)) <= S,
    del == 8*(N/G)*(C^3)/wire,
    Pmax*del + 1.05*(N + 2)*wire <= lmax,
    coil + wire <= coilmax,

    #allowable wire diameter
    table({wire},
      {0.207, 0.225, 0.244, 0.263, 0.283, 0.307, 0.331, 0.362, 0.394, 0.4375, 0.500}
    );
```
