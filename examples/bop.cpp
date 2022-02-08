#include <iostream>
#include "realpaver/BOPSolver.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try
   {
      Problem problem;

      Variable x = problem.addRealVar( 0,  1, "x"),
               y = problem.addRealVar( 1,  1, "y"),
               z = problem.addRealVar(-3, -3, "z");

      problem.addObjective(minimize(sqr(x) + pow(y,3) - x*y - 2*z));

      BOPSolver solver(problem);

      bool optimal = solver.optimize();

      if (optimal)
      {
         cout << "OPTIMAL"
              << endl
              << "Objective value: " << solver.getObjEnclosure()
              << endl
              << "Best solution found: " << solver.getBestSolution()
              << endl;
      }
      else
      {
         cout << "NOT OPTIMAL" << endl;
      }
   }
   catch(Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
