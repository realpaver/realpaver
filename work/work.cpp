#include <iostream>
#include "realpaver/Dag.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"

using namespace std;
using namespace realpaver;



Term dist(Term x1, Term y1, Term x2, Term y2)
{
   return sqrt(sqr(x1-x2) + sqr(y1-y2));
}

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 4 );

   try {

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
