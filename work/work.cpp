#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/BOModel.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 16 );

   Tolerance tol = Tolerance::makeRel(1.0e-6);
   Interval k = tol.maxIntervalUp(2.0);
   cout << "k = " << k << endl;
   
   if (tol.hasTolerance(k)) cout << "ok" << endl;
   else cout << "non" << endl;
   
return 0;

   try {
      Problem* prob = new Problem();
      Dag* dag = new Dag();
      Variable x = prob->addRealVar(0, 10, "x"),
               y = prob->addRealVar(2, 6, "y"),
               z = prob->addRealVar(-1, 3, "z");

      dag->insert( y - sqr(x) == 0 );
      dag->insert( sqr(x) + 2 == 0 );
      dag->insert( sqr(x) + sqr(y) - z == 0 );
      cout << *dag << endl;

      IntervalRegion reg = prob->getDomains();
      cout << "reg : " << reg << endl;
   
      dag->eval(reg);

      LPModel lpm;
      dag->linearize(lpm);

      cout << lpm << endl;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
