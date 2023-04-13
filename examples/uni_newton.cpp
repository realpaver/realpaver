#include <iostream>
#include "realpaver/Dag.hpp"
#include "realpaver/UniIntervalNewton.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/ThickFunction.hpp"

using namespace realpaver;
using namespace std;

Interval f(const Interval& x)  { return sqr(x) - 2.0; }
Interval df(const Interval& x) { return 2.0*x; }

int main(void)
{
   Logger::init(LogLevel::full, "newton.log");

   UniIntervalNewton newton;
   Interval I(1, 10);

   Proof p = newton.contract(f, df, I);
   cout << "Proof: " << p << endl;

   if (p != Proof::Empty)
      cout << "New domain: " << I << endl;

   return 0;
}

// [1, 10]
// [1, 4.087500000000001]
// [1, 1.996879778287463]
// [1.375778838455192, 1.437013531872623]
// [1.414068266834451, 1.414409744371276]
// [1.414213559529903, 1.414213565673288]
// Proof: feasible
