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
   Interval::precision( 12 );

   try {
      Problem prob;
      Variable x = prob.addRealVar(-2, 2, "x");
      Variable y = prob.addRealVar(-2, 2, "y");

      IntervalRegion reg = prob.getDomains();


      SharedDag dag = std::make_shared<Dag>();
      size_t i1 = dag->insert( y - sqr(x) == 0);
      size_t i2 = dag->insert( sqr(x) + sqr(y) - 2 == 0);

      cout << "input reg " << reg << endl;

      PolytopeHullContractor phc(dag, PolytopeCreatorStyle::Taylor);
      Proof proof = phc.contract(reg);

      cout << proof << " " << reg << endl;

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
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
