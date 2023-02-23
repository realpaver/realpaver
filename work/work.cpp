#include <iostream>
#include <fstream>
#include <iomanip>
#include "realpaver/Dag.hpp"
#include "realpaver/IntervalFunctionVector.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/PolytopeHullContractor.hpp"
#include "realpaver/Range.hpp"
#include "realpaver/VariableVector.hpp"
#include "realpaver/QuadraticTerm.hpp"
#include "realpaver/HC4Contractor.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 12 );

   try {
      Variable x("x");
      Variable y("y");

      IntervalRegion R({x, y});
      R.set(x, Interval(-4, 3));
      R.set(y, Interval(0, 7));

      IntervalFunction f(sqr(x) + y + 1.0);
      IntervalVector G(2);
      f.diff(R, G);
      cout << G << endl;
      cout << f.scope() << endl;
      cout << f.nbVars() << endl;

/*
      IntervalFunctionVector F({sqr(x) + y + 1.0, y - 3*x});

      F.eval(R);
      cout << F.getValues() << endl;

      IntervalMatrix J(2, 2);
      F.diff(R, J);
      cout << J << endl;
*/
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
