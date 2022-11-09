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
      //~ Variable x = prob.addRealVar(0,  3, "x");
      //~ Variable y = prob.addIntVar (4,  7, "y");
      //~ Variable z = prob.addIntVar (-2, 10, "z");

      Variable x = prob.addRealVar(0,  3, "x");
      Variable y = prob.addIntVar (4,  7, "y");
      Variable z = prob.addIntVar (-2, -1, "z");

      Scope sco = {z, x};
      cout << sco.var(1).getName() << endl;

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

      Constraint c = table( {x, y, z},
                            {1, 2, 3,
                             1, 5, 0,
                             2, 8, 3,
                             3, 6, 11} );
      cout << c << c.scope() << endl;

      IntervalRegion reg = prob.getDomains();
      cout << "reg : " << reg << endl;
      
      if (c.isSatisfied(reg) != Proof::Empty)
         cout << "satisfied" << endl;
      else
         cout << "not satisfied" << endl;

      cout << "violation : " << c.violation(reg) << endl;

      cout << "--------\nreg : " << reg << endl;
      
      RealPoint pt = reg.midpoint();
      cout << "pt : " << pt << endl;

      IntervalRegion ir(pt);
      cout << "ir : " << ir << endl;

      RealVector ptv(pt);
      cout << "ptv : " << ptv << endl;

      IntervalVector vi(ptv);
      cout << "vi : " << vi << endl;

      IntervalRegion rsr(reg.scope(), ptv);
      cout << "rsr : " << rsr << endl;


//      cout << c.contract(reg) << endl << reg << endl;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
