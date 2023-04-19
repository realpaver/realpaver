#include <iomanip>
#include <iostream>
#include "realpaver/LPSolver.hpp"

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
  
   /////////////////////////////////////////////////////////////////////////////
   // maximize 3x1 + 4x2
   LinExpr eo = { {3.0, 4.0}, {x1, x2} };
   solver.setObj(eo);
   solver.setMaximization();

   // SOLUTION: x* = (6, 4)
   //           y* = (7/3, 0, 1/3)
   //           f* = 3*6 + 4*3 = 7/3*14 + 1/3*2

   // solving
   bool optimal = solver.optimize();

   cout << std::setprecision(16);

   if (optimal)
   {
      cout << "OPTIMAL" << endl;

      // optimum
      cout << "f*:  " << solver.getSafeObjVal() << endl;

      // primal variables
      for (int i=0; i<solver.getNbLinVars(); ++i)
      {
         LinVar v = solver.getLinVar(i);
         cout << v.getName() << "*: " << v.getObjVal()
              << ", multiplier : " << v.getMultiplier()
              << endl;
      }

      // multipliers / dual variables
      for (int i=0; i<solver.getNbLinCtrs(); ++i)
      {
         LinCtr c = solver.getLinCtr(i);
         cout << "y" << i+1 << "*: " << c.getMultiplier() << endl;
      }
   }
   else
   {
      cout << "NOT OPTIMAL" << endl;
   }

   cout << endl;

   /////////////////////////////////////////////////////////////////////////////
   // minimize 0.25*x1 - x2
   LinExpr neo = { {0.25, -1.0}, {x1, x2} };
   solver.setObj(neo);
   solver.setMinimization();

   // SOLUTION: f* = -5.5 at x* = (2, 6)
   //      with y* = (-0.393, 0.214, 0)

   // solving
   optimal = solver.reoptimize();

   if (optimal)
   {
      cout << "OPTIMAL" << endl;

      // optimum
      cout << "f*:  " << solver.getSafeObjVal() << endl;

      // primal variables
      for (int i=0; i<solver.getNbLinVars(); ++i)
      {
         LinVar v = solver.getLinVar(i);
         cout << v.getName() << "*: " << v.getObjVal()
              << ", multiplier : " << v.getMultiplier()
              << endl;
      }

      // multipliers / dual variables
      for (int i=0; i<solver.getNbLinCtrs(); ++i)
      {
         LinCtr c = solver.getLinCtr(i);
         cout << "y" << i+1 << "*: " << c.getMultiplier() << endl;
      }
   }
   else
   {
      cout << "NOT OPTIMAL" << endl;
   }

   return 0;
}
