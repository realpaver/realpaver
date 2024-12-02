# How to solve a constraint satisfaction problem

## Using the command-line

The solver takes two files as input describing the parameters and a problem, namely 'param.txt' with prefix '-p' and '2r-robot.rp', as follows:

``` bash
ncsp_solver -p param.txt 2r-robot.rp
```

The following problem file describes a 2R planar robot.

``` py
# 2R planar robot with 2 revolute joints
Constants
   l1 = 4.5,               # length of link 1
   l2 = 3.0;               # length of link 2

Variables
   q1 in [-PI, +PI],       # angle of link 1
   q2 in [-PI, +PI],       # angle of link 2
   x in [-10, 10],         # abcissa of end effector
   y in [-10, 10];         # ordinate of end effector

Constraints
   # general
   x == l1*cos(q1) + l2*cos(q1+q2),
   y == l1*sin(q1) + l2*sin(q1+q2),

   # inverse kinematics
   x == 5.75, y == 4.25;
```

## Using the C++ API

The 2R planar robot described above can be processed by a C++ program.

It is necessary to include the main header of the API and to use the namespace.

``` cpp
#include "realpaver_api.hpp"
using namespace realpaver;
```

A try/catch block (e.g. in a main function) contains the application code.

``` cpp
try {
   // application (code given below)
}
catch(Exception e) {
   cout << e.what() << endl;
}
```

Parameters are loaded from the given file.

``` cpp
Param prm;
prm.loadParam("param.txt");
```

A problem instance is created.

``` cpp
Problem problem;
```

The problem file is parsed.

``` cpp
Parser parser(prm);
bool ok = parser.parseFile("2r-robot.rp", problem);
```

The solver is called.

``` cpp
NcspSolver solver(problem);
solver.getEnv()->setParam(prm);
solver.solve();
```

The solutions are printed.

``` cpp
for (size_t i=0; i<solver.nbSolutions(); ++i)
{
   pair<DomainBox, Proof> sol = solver.getSolution(i);
   cout << "SOL " << i << ": " << sol.first
        << " (" << sol.second << ")" << endl;
}
```

It is also possible to define the problem without calling the parser.

``` cpp
// Variables
double l1 = 4.5, l2 = 3.0;
Variable q1 = problem.addRealVar(Interval::minusPiPlusPi(), "q1"),
         q2 = problem.addRealVar(Interval::minusPiPlusPi(), "q2"),
         x = problem.addRealVar(-10, 10, "x"),
         y = problem.addRealVar(-10, 10, "y");

// Constraints
problem.addCtr({ x == l1*cos(q1) + l2*cos(q1+q2),
                 y == l1*sin(q1) + l2*sin(q1+q2),
                 x == 5.75,
                 y == 4.25});
```
