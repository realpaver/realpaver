#include <iostream>
#include <fstream>
#include <iomanip>
#include "realpaver/Dag.hpp"
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
      // TODO
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
