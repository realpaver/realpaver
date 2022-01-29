#include <iostream>
#include "realpaver/dag.hpp"
#include "realpaver/interval_newton.hpp"
#include "realpaver/problem.hpp"
#include "realpaver/thick_fun.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   Problem problem;
   Variable x = problem.addRealVar(1.0, 10.0, "x");
   Constraint c( sqr(x) - 2.0 == 0.0 );

   Dag dag;
   size_t idx = dag.insert(c);
   ThickFun f(&dag, idx, x.id());

   IntervalNewton newton;
   Interval I( x.domain() );

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
