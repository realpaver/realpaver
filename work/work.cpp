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
      Variable x = prob.addRealVar(1, 2, "x");
      Variable y = prob.addBoolVar("y");
      Variable z = prob.addIntVar(3, 7, "z");

      Constraint c = table( {x, y, z},
                            {1, 2, 3,
                             4, 5, 6,
                             7, 8, 9,
                             10, 11, 12,
                             13, 14, "1.1"} );
      cout << c;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
