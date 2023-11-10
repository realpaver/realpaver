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

   try {
      Problem P;
      Variable x = P.addRealVar(0, 1, "x");
      Variable y = P.addRealVar(3, 4, "y");
      
      Term t(pow(x, y));
      cout << t << endl;

   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
