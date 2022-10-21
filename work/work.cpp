#include <iostream>
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
      Variable x = prob.addRealVar(0,  3, "x");
      Variable y = prob.addIntVar (4,  7, "y");
      Variable z = prob.addIntVar (-2, 10, "z");

      Constraint c = table( {x, y, z},
                            {1, 2, 3,
                             1, 5, 0.0,
                             2, 8, 3,
                             3, 6, 11} );
      cout << c << c.scope() << endl;

      IntervalRegion reg = prob.getDomains();
      cout << "reg : " << reg << endl;
      
      if (c.isSatisfied(reg) != Proof::Empty)
         cout << "satisfied" << endl;
      else
         cout << "not satisfied" << endl;


      cout << c.contract(reg) << endl << reg << endl;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
