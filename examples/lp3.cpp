#include "LPModel.hpp"
#include "realpaver_api.hpp"
#include <iomanip>
#include <iostream>

using namespace realpaver;
using namespace std;

int main(void)
{
   LPSolver solver;
   LinVar x1 = solver.makeVar(-10.0, 10.0, "x1"), x2 = solver.makeVar(-10.0, 10.0, "x2"),
          x3 = solver.makeVar(-10.0, 10.0, "x3"), x4 = solver.makeVar(-10.0, 10.0, "x4"),
          x5 = solver.makeVar(-10.0, 10.0, "x5");

   LinExpr e1 = {{2.0, 1.0, 1.0, 1.0, 1.0}, {x1, x2, x3, x4, x5}};
   solver.addCtr(6.0, e1, 6.0);

   LinExpr e2 = {{1.0, 2.0, 1.0, 1.0, 1.0}, {x1, x2, x3, x4, x5}};
   solver.addCtr(6.0, e2, 6.0);

   LinExpr e3 = {{1.0, 1.0, 2.0, 1.0, 1.0}, {x1, x2, x3, x4, x5}};
   solver.addCtr(6.0, e3, 6.0);

   LinExpr e4 = {{1.0, 1.0, 1.0, 2.0, 1.0}, {x1, x2, x3, x4, x5}};
   solver.addCtr(6.0, e4, 6.0);

   double a = 10000.0;

   LinExpr e5 = {{-a, a, -a, a, -a}, {x1, x2, x3, x4, x5}};
   solver.addCtr(e5, 600002.0);

   LinExpr e6 = {{a, -a, a, -a, a}, {x1, x2, x3, x4, x5}};
   solver.addCtr(-399999.0, e6);

   LinExpr e7 = {{a, -a, a, -a, a}, {x1, x2, x3, x4, x5}};
   solver.addCtr(e7, 400002.0);

   LinExpr e8 = {{-a, a, -a, a, -a}, {x1, x2, x3, x4, x5}};
   solver.addCtr(-599999.0, e8);

   LinExpr cost = {{1.0}, {x3}};
   solver.setCost(cost);
   solver.setSense(LPSense::Min);

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
