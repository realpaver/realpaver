#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/LocalOptimizer.hpp"
#include "realpaver/Param.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try {
      Problem problem;
      Variable x = problem.addRealVar(0, 10, "x"),
               y = problem.addRealVar(0, 10, "y");

      RealFunction f(sqr(x) + sqr(y));
      RealFunctionVector G( {y - x + 1, y - x - 2},
                            {Interval::negative(), Interval::positive()} );

      DefaultLocalOptimizer optimizer(f, G);


   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
