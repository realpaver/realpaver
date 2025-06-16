#include "realpaver_api.hpp"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace realpaver;

int main(void)
{
   try
   {
      // Defines the NCSP modelling the intersection of circles and a parabola
      Problem problem;

      Variable x = problem.addRealVar(-7, 3, "x"), y = problem.addRealVar(-6, 4, "y"),
               n = problem.addIntVar(0, 6, "n");

      problem.addCtr({sqr(x) + sqr(y) == sqr(n), y == sqr(x) + 1.0});

      // Solves the NCSP
      CSPSolver solver(problem);

      Timer tim;
      tim.start();
      solver.solve();
      tim.stop();

      // Prints the solutions
      for (size_t i = 0; i < solver.nbSolutions(); ++i)
      {
         std::pair<DomainBox, Proof> sol = solver.getSolution(i);
         cout << "Sol " << (i + 1) << ": " << setprecision(8) << sol.first << endl;
      }
      cout << "Elapsed time: " << tim.elapsedTime() << " (s)" << endl;
   }
   catch (Exception ex)
   {
      cout << ex.what() << endl;
   }

   return 0;
}
