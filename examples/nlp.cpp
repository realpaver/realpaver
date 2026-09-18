#include "Objective.hpp"
#include "realpaver/NLPSolver.hpp"
#include "realpaver_api.hpp"
#include <cmath>
#include <iostream>

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::full, "nlp.log");

   try
   {
      Problem problem;
      Variable x1 = problem.addRealVar(-2, 2, "x1"), x2 = problem.addRealVar(-3, 3, "x2");

      Variable vobj = problem.addRealVar(-INFINITY, INFINITY, "__obj__");
      Objective obj = minimize(vobj, 100 * sqr(x2 - sqr(x1)) + sqr(1 - x1));
      problem.addObjective(obj);
      problem.addCtr(sqr(x1) + sqr(x2) <= 2);

      NLPSolver optimizer(problem);
      optimizer.setAlgorithm("NLOPT_SLSQP");
      optimizer.setIterLimit(1e4);
      optimizer.setTimeLimit(10);

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
