# How to solve a constrained optimization problem

## Using the command-line

When building the RealPaver library, a default COP solver is also generated, namely `rp_optimizer`. It is able to solve COPs written using the [RealPaver dedicated language](language.md).

From the command line, the optimizer requires the file path to the problem desciption written in the RealPaver language. Thus, solving a COP relating to a Beam sizing problem, described in the file 'beam.rp', can be solved as follows:

``` bash
# Run with default parameters
rp_optimizer beam.rp
```

[RealPaver parameters](parameters.md) can be specified with prefix `-p` in a dedicated text file to adapt the behavior of the optimizer. For instance, the optimizer can be called to solve again the beam sizing problem with some parameters overloaded by the definitions in 'param.txt', as follows:

``` bash
# Run with specific parameters in param.txt
rp_solver -p param.txt beam.rp
```

The following problem file describes a beam sizing problem:

``` py
Constants
  L = 4, # Length
  P = 1e4; # Force

Variables
  H in [0.1,0.8], # Height
  W in  [0.1,0.6], # Width
  T_f in [0.01,0.05], # Flange Thickness
  T_w in [0.01,0.05], # Web Thickness

  w_max in [0,5],  # Maximal displacement
  sigma_max in [0,1e8], # Density
  E in [0,1e15], # Maximal stress

  rho in [0,1e5]; # Elastic module

Aliases
  A = H*W - (H-2*T_w)*(W-T_f), # Area
  M = L * A * rho, # Mass
  Y = H/2,
  I_x = ((H-2*T_w)^3*T_f + 2*T_w^3*W + 6*T_w*W*(H-T_w)^2)/12,
  I_y = (2*T_w*W^3 + (H - 2*T_w)*T_f^3)/12,
  I = I_x + I_y; # Momentum of inertia

Minimize w_max;

Constraints
  w_max == P*L^3/(48*E*I_x),
  sigma_max == P*Y / (4*I_x),
  sigma_max <= 12.8e6,

  table({rho,E},{
    7800, 2.0e11, # Steel
    2700, 6.7e10, # Aluminium
    4400, 1.1e11, # Titanium
    8800, 1.06e11 # Bronze
  });
```

The following parameter file overloads the default values for the corresponding parameters:

``` bash
# in seconds
TIME_LIMIT=100

# Display traces in terminal
TRACE=YES

# Absolute tolerance on the width of intervals
OBJ_ABS_TOL=1e-8

# Polytope hull relaxation technique
POLYTOPE_HULL_RELAXATION=AFFINE

# Polytope hull relaxation technique
PROSPECTOR=INNER_AFFINE
```

## Using the C++ API

It is necessary to include the main header of the API and to use the namespace.

``` cpp
#include "realpaver_api.hpp"
using namespace realpaver;
```

Then, before calling the optimizer, the problem has to be defined and there are 2 possibilities: using the RealPaver modeling language or using the C++ API.

### Describing the problem with the RealPaver language

The beam sizing problem described above can be processed by a C++ program.

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
Params prm;                      // Default parameters
prm.loadParam("param.txt");      // Optional: overloading some parameters
```

An empty problem instance is created:

``` cpp
Problem problem;
```

Then, the problem is populated using a string decribing the beam problem, then it is directly used by the parser:

``` cpp
std::string rp_beam_str = R"RP(
  Constants
    L = 4, # Length
    P = 1e4; # Force
  Variables
    H in [0.1,0.8], # Height
    W in  [0.1,0.6], # Width
    T_f in [0.01,0.05], # Flange Thickness
    T_w in [0.01,0.05], # Web Thickness
    w_max in [0,5],  # Maximal displacement
    sigma_max in [0,1e8], # Density
    E in [0,1e15], # Maximal stress
    rho in [0,1e5]; # Elastic module
  Aliases
    A = H*W - (H-2*T_w)*(W-T_f), # Area
    M = L * A * rho, # Mass
    Y = H/2,
    I_x = ((H-2*T_w)^3*T_f + 2*T_w^3*W + 6*T_w*W*(H-T_w)^2)/12,
    I_y = (2*T_w*W^3 + (H - 2*T_w)*T_f^3)/12,
    I = I_x + I_y; # Momentum of inertia
  Minimize w_max;
  Constraints
    w_max == P*L^3/(48*E*I_x),
    sigma_max == P*Y / (4*I_x),
    sigma_max <= 12.8e6,
    table({rho,E},{
      7800, 2.0e11, # Steel
      2700, 6.7e10, # Aluminium
      4400, 1.1e11, # Titanium
      8800, 1.06e11 # Bronze
    });
   )RP";
Parser parser;
parser.parseStr(rp_beam_str, problem);
```

The optimizer is created for the problem. The parameters are applied to the solver inner context, i.e. its environment, and finally it is called to solve the problem:

``` cpp
Optimizer optimizer(problem);
optimizer.getEnv()->setParams(prm);
optimizer.minimize();
```

The best computed solution can be printed:

``` cpp
cout << "Optimal solution:" << optimizer.bestFeasible() << endl;
cout << "Optimal value enclosure:" << optimizer.getOptimum() << endl;
```

Additional information on the solving process can also be printed:

``` c++
cout << "Number of nodes: " << optimizer.getNbNodes() << endl;
cout << "Time used: " << optimizer.getSolvingTime() << endl;
```

## A complete example file

``` c++
#include <iostream>
using namespace std;

#include "realpaver_api.hpp"
using namespace realpaver;

int main()
{
   try
   {
      Params params;   // Default parameters
      Problem problem; // Empty problem
      std::string rp_beam_str = R"RP(
        Constants
          L = 4, # Length
          P = 1e4; # Force
        Variables
          H in [0.1,0.8], # Height
          W in  [0.1,0.6], # Width
          T_f in [0.01,0.05], # Flange Thickness
          T_w in [0.01,0.05], # Web Thickness
          w_max in [0,5],  # Maximal displacement
          sigma_max in [0,1e8], # Density
          E in [0,1e15], # Maximal stress
          rho in [0,1e5]; # Elastic module
        Aliases
          A = H*W - (H-2*T_w)*(W-T_f), # Area
          M = L * A * rho, # Mass
          Y = H/2,
          I_x = ((H-2*T_w)^3*T_f + 2*T_w^3*W + 6*T_w*W*(H-T_w)^2)/12,
          I_y = (2*T_w*W^3 + (H - 2*T_w)*T_f^3)/12,
          I = I_x + I_y; # Momentum of inertia
        Minimize w_max;
        Constraints
          w_max == P*L^3/(48*E*I_x),
          sigma_max == P*Y / (4*I_x),
          sigma_max <= 12.8e6,
          table({rho,E},{
            7800, 2.0e11, # Steel
            2700, 6.7e10, # Aluminium
            4400, 1.1e11, # Titanium
            8800, 1.06e11 # Bronze
          });
         )RP";
      Parser parser;
      parser.parseStr(rp_beam_str, problem);

      Optimizer optimizer(problem);
      optimizer.getEnv()->setParams(params);
      optimizer.optimize();
      cout << "Otpimization status: " << optimizer.getStatus() << endl;
      if (optimizer.isSolvedOptimal())
      {
         cout << "Optimal solution:" << optimizer.bestFeasible() << endl;
         cout << "Optimal value enclosure:" << optimizer.getOptimum() << endl;
      }
      cout << "Number of nodes: " << optimizer.getNbNodes() << endl;
      cout << "Time used: " << optimizer.getSolvingTime() << endl;
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
```
