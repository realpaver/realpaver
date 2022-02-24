#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/BOLocalConjugate.hpp"
#include "realpaver/dag.hpp"
#include "realpaver/Problem.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::internal, "work.log");
   Interval::precision( 16 );

   try {

      Problem P;
      Variable x = P.addRealVar(-4, 6, "x");
      Variable y = P.addRealVar(-3, 8, "y");

      IntervalVector initRegion = P.getDomains();
      RealVector initPoint = initRegion.midpoint();
      RealVector finalPoint(initPoint.size());

      Dag D;
      size_t i = D.insert( 1.5*sqr(x) + 3.0*sqr(y) + 2.0*x*y -2.0*x + 8.0*y );

      DagFun* fi = D.fun(i);

      BOLocalConjugate solver;

      OptimizationStatus status = solver.minimize(*fi, initRegion, initPoint, finalPoint);

      cout << status << endl << finalPoint << endl;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }


   return 0;
}
