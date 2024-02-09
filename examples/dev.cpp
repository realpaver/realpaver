#include <iostream>
#include "realpaver/ContractorHC4.hpp"
#include "realpaver/ContractorVar3B.hpp"
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/DomainSlicerFactory.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Timer.hpp"
#include "realpaver/Tolerance.hpp"
#include "realpaver/IntervalThickFunction.hpp"
#include "realpaver/IntervalNewtonUni.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   try
   {
      Problem P;
      Variable x = P.addRealVar(-3, 0, "x");

      SharedDag dag = make_shared<Dag>();
      dag->insert(sqr(x) - 2 == 0);
      IntervalThickFunction f(dag, 0, x);

      IntervalNewtonUni nwt;
      Interval I(3, 3);
      Proof p = nwt.localSearch(f, I);
      cout << p << " " << I << endl;
      
/*
      Tolerance tol(0.5, 2);
      
      Interval x(10, 20);
      cout << tol.isTight(x) << endl;
*/

/*
      Problem P;
      Variable x = P.addRealVar(-3, 0, "x"),
               y = P.addRealVar(0, 5, "y"),
               z = P.addRealVar(-1e08, 1e08, "z");

      P.addCtr( sqr(y) * (1 + sqr(z)) + z * (z - 24 * y) == -13 );
      P.addCtr( sqr(x) * (1 + sqr(y)) + y * (y - 24 * x) == -13 );
      P.addCtr( sqr(z) * (1 + sqr(x)) + x * (x - 24 * z) == -13 );

      std::shared_ptr<Env> env = std::make_shared<Env>();
      ContractorFactory factory(P, env);
      SharedDag dag = factory.getDag();

      ContractorHC4 h0(dag, 0);
      ContractorHC4 h1(dag, 1);
      ContractorHC4 h2(dag, 2);

      Scope S( {x, y, z} );
      IntervalBox B(S);
      cout << "B =: " << B << endl;

      Proof proof = h2.contract(B);
      cout << proof << " " << B << endl;

      int NS = 100000;
      Timer tim;
      tim.start();
      for (int i=0; i<NS; ++i)
      {
         B.set(x, Interval(-3, 0));
         B.set(y, Interval(0, 5));
         B.set(z, Interval(-1e08, 1e08));
         h2.contract(B);
      }
      tim.stop();
      cout << tim.elapsedTime() << " (s)" << endl;

      Constraint c2 = P.ctrAt(2);
      Timer tom;
      tom.start();
      for (int i=0; i<NS; ++i)
      {
         B.set(x, Interval(-3, 0));
         B.set(y, Interval(0, 5));
         B.set(z, Interval(-1e08, 1e08));
         c2.contract(B);
      }
      tom.stop();
      cout << tom.elapsedTime() << " (s)" << endl;
*/
   }
   catch(Exception e)
   {
      cout << e.message() << endl;
   }
   return 0;
}
