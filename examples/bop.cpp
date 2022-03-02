#include <iostream>
#include "realpaver/BOSolver.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::internal, "bop.log");
   Interval::precision( 16 );

   try {
      Param::init("../src/realpaver/settings.txt");
      Param::print(cout);
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   try {
      Problem problem;

      Variable x = problem.addRealVar(-10,  4, "x"),
               u = problem.addRealVar(-10,  10, "u"),
               y = problem.addRealVar( -1,  2, "y");

      problem.addObjective(minimize(3*u + sqr(x)*sqr(y) + x*y));

      BOSolver solver(problem);
      solver.setNodeLimit(5);

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
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
