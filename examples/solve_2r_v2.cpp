#include <iostream>
using namespace std;

#include "realpaver_api.hpp"
using namespace realpaver;

int main()
{
   try
   {
      Param prm;
      prm.loadParam("../../examples/param.txt");

      Problem problem;

      // Variables
      double l1 = 4.5, l2 = 3.0;
      Variable q1 = problem.addRealVar(Interval::minusPiPlusPi(), "q1"),
               q2 = problem.addRealVar(Interval::minusPiPlusPi(), "q2"),
               x = problem.addRealVar(-10, 10, "x"), y = problem.addRealVar(-10, 10, "y");

      // Constraints
      problem.addCtr({x == l1 * cos(q1) + l2 * cos(q1 + q2),
                      y == l1 * sin(q1) + l2 * sin(q1 + q2), x == 5.75, y == 4.25});

      CSPSolver solver(problem);
      solver.getEnv()->setParam(prm);
      solver.solve();
      for (size_t i = 0; i < solver.nbSolutions(); ++i)
      {
         pair<DomainBox, Proof> sol = solver.getSolution(i);
         cout << "SOL " << i << ": " << sol.first << " (" << sol.second << ")" << endl;
      }
      cout << solver.nbSolutions() << " solutions found in " << solver.getSolvingTime()
           << " s, using " << solver.getTotalNodes() << " nodes." << endl;
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
