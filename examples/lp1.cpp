#include <iomanip>
#include <iostream>
#include "realpaver_api.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   LPSolver solver;

   // x1 and x2 are continuous variables lying in [0, 10]
   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"),
          x2 = solver.makeVar(0.0, 10.0, "x2");

   // x1 + 2x2 <= 14
   LinExpr e1 = { {1.0, 2.0}, {x1, x2} };
   solver.addCtr(e1, 14.0);

   // 3x1 - x2 >= 0
   LinExpr e2 = { {3.0, -1.0}, {x1, x2} };
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = { {1.0, -1.0}, {x1, x2} };
   solver.addCtr(e3, 2.0);

   // maximize 3x1 + 4x2
   LinExpr cost = { {3.0, 4.0}, {x1, x2} };
   solver.setCost(cost);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (6, 4)
   //           f* = 34

   // solving
   // solving
   LPStatus status = solver.optimize();

   if (status == LPStatus::Optimal)
   {
      double cc = solver.certifiedCostSolution();
      cout << setprecision(16);

      cout << "Optimal solution found" << endl;
      cout << "  cost:           " << solver.costSolution() << endl;
      cout << "  certified cost: " << cc << endl;
      cout << "  primal:         " << solver.primalSolution() << endl;
      cout << "  dual:           " << solver.dualSolution() << endl;
   }
   else
   {
      cout << "NOT OPTIMAL" << endl;
   }

   return 0;
}
