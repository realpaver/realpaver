#include <iostream>
#include "realpaver/Dag.hpp"
#include "realpaver/IntervalNewtonUni.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/IntervalFunctionUni.hpp"

using namespace realpaver;
using namespace std;

class MyFun : public IntervalFunctionUni {
   Interval eval(const Interval& x)  { return sqr(x) - 2.0; }
   Interval diff(const Interval& x) { return 2.0*x; }
};

int main(void)
{
   Logger::init(LogLevel::full, "newton.log");

   IntervalNewtonUni newton;
   MyFun f;
   Interval I(1, 10);

   Proof p = newton.contract(f, I);
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
