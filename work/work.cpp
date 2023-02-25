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
#include "realpaver/RealMatrix.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 12 );

   try {

      //~ RealMatrix M({{-1.0, 2.0, 3.5},
                    //~ {1.25, 0.0, -2.5},
                    //~ {-0.5, 0.0, 6.0}});

      RealMatrix M({{1, -1},
                    {-2, 1}});

      RealMatrix MM(M);

      RealMatrix P(M);
      P.setIdentity();

      cout << "--- M ---" << endl << M << endl << endl;
      cout << "--- P ---" << endl << P << endl << endl;

      bool b = M.inverse(P);
      if (b)
      {
         cout << "--- M ---" << endl << M << endl << endl;
         cout << "--- P ---" << endl << P << endl << endl;
         cout << "--- P*M ---" << endl << P*MM << endl << endl;
      }
      else
         cout << "SINGULAR" << endl;

/*
      Variable x("x");
      Variable y("y");

      IntervalRegion R({x, y});
      R.set(x, Interval(-4, 3));
      R.set(y, Interval(0, 7));
      cout << R << endl;

      Term ta(sqr(x) - y);
      Term tb(x + 2*y);
      Term tc(abs(x) - y + 1.0);

      IntervalFunctionVector F({ta, tb});
      cout << "scope : " << F.scope() << endl;
      cout << "nb V  : " << F.nbVars() << endl;
      cout << "nb F : " << F.nbFuns() << endl;

      F.addFun(IntervalFunction(tc));
      cout << "scope : " << F.scope() << endl;
      cout << "nb V  : " << F.nbVars() << endl;
      cout << "nb F : " << F.nbFuns() << endl;
   
      IntervalVector V(F.nbFuns());
      F.eval(R, V);
      cout << "eval : " << V << endl;

      IntervalMatrix J(F.nbFuns(), F.nbVars());
      F.diff(R, J);
      cout << "diff : " << endl << J << endl;
*/
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
