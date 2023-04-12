#include <signal.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Problem.hpp"

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
