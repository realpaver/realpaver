#include <iostream>
#include "realpaver/BOSolver.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try
   {
      Problem problem;

      Variable x = problem.addRealVar(-10,  4, "x"),
               u = problem.addRealVar(-10,  10, "u"),
               y = problem.addRealVar( -1,  2, "y");

      problem.addObjective(minimize(3*u + sqr(x) + x*y + sqr(y)));

      BOSolver solver(problem);

      bool optimal = solver.optimize();

      cout << "Optimization status:    " << solver.getStatus() << endl;

      if (optimal)
      {
         cout << "Objective value:     " << solver.getObjEnclosure()
              << endl
              << "Best solution found: " << solver.getBestSolution()
              << endl;
      }
   }
   catch(Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
