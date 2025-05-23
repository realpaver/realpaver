#include "realpaver/NLPSolver.hpp"
#include "realpaver_api.hpp"
#include <iostream>

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::full, "nlp.log");

   try
   {
      Problem problem;
      Variable x = problem.addRealVar(1, 10, "x"), y = problem.addRealVar(-1, 9, "y"),
               w = problem.addRealVar(0, 9, "w"), z = problem.addRealVar(2, 7, "z");

      RealFunction f(sqr(x) + sqr(z));

      RealFunctionVector G({y - z + 1, z - x - 2},
                           {Interval::negative(), Interval::positive()});

      NLPSolver optimizer(f, G);
      optimizer.setAlgorithm("NLOPT_SLSQP");

      IntervalBox box(problem.scope());
      RealPoint src = box.midpoint();

      OptimizationStatus status = optimizer.minimize(box, src);
      cout << "Status.......... " << status << endl;

      if (status == OptimizationStatus::Optimal)
      {
         cout << "Optimum value... " << optimizer.bestVal() << endl
              << "at point........ " << optimizer.bestPoint() << endl;
      }
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
