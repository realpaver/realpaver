#include <iostream>
#include "realpaver/lpsolver.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   LPSolver solver;

   // x and y are continuous variables lying in [0, 10]
   LinVar x = solver.makeVar(0.0, 10.0, "x"),
          y = solver.makeVar(0.0, 10.0, "y");

   // x + 2y <= 14
   LinExpr e1 = { {1.0, 2.0}, {x, y} };
   solver.addCtr(e1, 14.0);

   // 3x - y >= 0
   LinExpr e2 = { {3.0, -1.0}, {x, y} };
   solver.addCtr(0.0, e2);

   // x - y <= 2
   LinExpr e3 = { {1.0, -1.0}, {x, y} };
   solver.addCtr(e3, 2.0);
  
   // maximize 3x + 4y
   LinExpr eo = { {3.0, 4.0}, {x, y} };
   solver.setObj(eo);
   solver.setMaximization();

   // solving
   bool optimal = solver.optimize();

   if (optimal)
   {
      cout << "OPTIMAL" << endl;
      cout << "Objective value: " << solver.getObjVal() << endl;

      for (int i=0; i<solver.getNbLinVars(); ++i)
      {
         LinVar v = solver.getLinVar(i);
         cout << v.getName() << "*: " << v.getObjVal() << endl;
      }
   }
   else
   {
      cout << "NOT OPTIMAL" << endl;
   }

   return 0;
}
