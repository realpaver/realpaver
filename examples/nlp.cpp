#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/NLPSolver.hpp"
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

      NLPSolver optimizer(f, G);

      IntervalRegion reg = problem.getDomains();
      RealPoint src = reg.midpoint();

      OptimizationStatus status = optimizer.minimize(reg, src);
      cout << "Status.......... " << status << endl;
   
      if (status == OptimizationStatus::Optimal)
      {
         cout << "Optimum value... " << optimizer.bestVal() << endl
              << "at point........ " << optimizer.bestPoint() << endl;
      }
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
