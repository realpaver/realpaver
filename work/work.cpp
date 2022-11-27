#include <iostream>
#include <fstream>
#include "realpaver/Dag.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/PolytopeHullContractor.hpp"
#include "realpaver/Range.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 6 );

   try {
      
      Range r(0, 10);
      cout << r << endl;
      cout << r * Range::universe() << endl;
      
      return 1;
      
      Problem prob;
      Variable x = prob.addRealVar(1, 2, "x");
      Variable y = prob.addRealVar(-1, 3, "y");

      IntervalRegion reg = prob.getDomains();

      SharedDag dag = std::make_shared<Dag>();
      size_t i1 = dag->insert( y - sqr(x) == 0);
      size_t i2 = dag->insert( sqr(x) + sqr(y) == 2);

      cout << (*dag);
      cout << dag->scope() << endl;
      cout << "input reg " << reg << endl;

      PolytopeHullContractor phc(dag, PolytopeCreatorStyle::RLT);
      Proof proof = phc.contract(reg);

      cout << proof << " " << reg << endl;

/*
   LPSolver solver;
   double largeur = 311.5, L = 109.2, S = largeur - 2*L, lo = 0;

   // variables
   vector<LinVar> A, B;
   int nc = 4;

   for (size_t i=0; i<nc; ++i)
   {
      ostringstream aos;
      aos << "a" << i;
      A.push_back(solver.makeVar(lo, L, aos.str()));

      ostringstream bos;
      bos << "b" << i;
      B.push_back(solver.makeVar(lo, L, bos.str()));
   }

   // Ai + Bi = S
   for (size_t i=0; i<nc; ++i)
   {
      LinExpr e = { {1.0, 1.0}, {A[i], B[i]} };
      solver.addCtr(S, e, S);
   }

   // Bi + Ai+1 = L
   for (size_t i=0; i<nc-1; ++i)
   {
      LinExpr e = { {1.0, 1.0}, {B[i], A[i+1]} };
      solver.addCtr(L, e, L);
   }

   // perte
   LinVar p = solver.makeVar(0, 2*L, "p");

   // perte : p = L - A0 + L - Bnc-1 <=> p + A0 + Bnc-1 = 2L
   LinExpr ep = { {1.0, 1.0, 1.0}, {p, A[0], B[nc-1]} };
   solver.addCtr(2*L, ep, 2*L);

   // maximize A0
   LinExpr ob = { {1.0}, {A[0]} };
   solver.setObj(ob);
   solver.setMaximization();

   cout << solver << endl;

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

   cout << endl;
*/
/*
      Problem prob;
      Variable x = prob.addRealVar(1, 2, "x");
      Variable y = prob.addRealVar(0, 0, "y");
      
      Term t( sqr(x) - 4*x*y + sqr(y) );
      IntervalRegion reg = prob.getDomains();
      
      cout << t.eval(reg) << endl;
*/


/*
      double x1 = 2,   y1 = 1.5, d1 = 2.55,
             x2 = 6,   y2 = 5.5, d2 = 3.81,
             x3 = 8.5, y3 = 1,   d3 = 4.12,
             E = 1.0e-2;


      Problem prob;
      Variable xM = prob.addRealVar(-1.0e8, 1.0e8, "xM");
      Variable yM = prob.addRealVar(-1.0e8, 1.0e8, "yM");

      prob.addCtr({ in(dist(xM, yM, x1, y1), Interval(d1-E, d1+E)),
                    in(dist(xM, yM, x2, y2), Interval(d2-E, d2+E)),
                    in(dist(xM, yM, x3, y3), Interval(d3-E, d3+E)) });

      NcspSolver solver(prob);

      Param prm;
      prm.loadParam("ParamFile");
      solver.getEnv()->setParam(prm);

      solver.solve();
*/
/*
      IntervalRegion reg = prob.getDomains();

      Dag dag;
      dag.insert(sqr(x) - sqr(y));
      dag.insert(3.0*x + sqr(z));
      dag.insert(sqr(z) + z);
      
      cout << dag << endl;
      IntervalMatrix J(3, 3);
      dag.intervalDiff(reg, J);
      cout << J << endl;

      RealPoint pt = reg.midpoint();
      RealMatrix R(3, 3);
      dag.realDiff(pt, R);
      cout << R << endl;
*/

   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
