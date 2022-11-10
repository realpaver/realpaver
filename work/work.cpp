#include <iostream>
#include "realpaver/Dag.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 4 );

   try {
      Problem prob;
      Variable x = prob.addRealVar(0, 10, "x");
      Variable y = prob.addRealVar(0, 10, "y");

      Term t = sqr(x) - 4*x*y + sqr(y);
;
      cout << "t : " << t << endl;

      IntervalRegion reg = prob.getDomains();
      cout << "reg : " << reg << endl;

      cout << "t(reg) : " << t.eval(reg) << endl;

      Constraint c( t >= 0.0);
      cout << c.contract(reg) << " " << reg << endl;


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
