#include <iostream>
#include "realpaver/ContractorHC4Revise.hpp"
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


// meson setup build -DLP_LIB=Highs -DASSERT=true -DLOG=true -Dbuildtype=debug
// meson setup build -DLP_LIB=Highs -DASSERT=false -DLOG=false -Dbuildtype=release

int main(void)
{
   try
   {
      Problem P;
      Variable x = P.addRealVar(2, 2, "x"),
               y = P.addRealVar(-2, 2, "y"),
               z = P.addRealVar(-1e08, 1, "z"),
               n = P.addIntVar(1, 8, "n"),
               m = P.addIntVar(1, 8, "m");

      Constraint guard( x == 2 ),
                 body( y - z == 0 ),
                 c( cond(guard, body) );

      cout << c << endl;
      
      IntervalBox B( c.scope() );
      cout << B << endl;

      cout << c.isSatisfied(B) << endl;
      cout << c.contract(B) << endl;
      cout << B << endl;

      //P.addCtr( sqr(y) * (1 + sqr(z)) + z * (z - 24 * y) == -13 );
      //P.addCtr( sqr(x) * (1 + sqr(y)) + y * (y - 24 * x) == -13 );
      //P.addCtr( sqr(z) * (1 + sqr(x)) + x * (x - 24 * z) == -13 );


/*
      P.addCtr( 11*pow(x,8) - 21*pow(y,6) + 44*sqr(x)*pow(y,6) + 11*pow(y,8) == 0);
      P.addCtr( sqr(x) + sqr(y) - 1 == 0 );

      std::shared_ptr<Env> env = std::make_shared<Env>();
      ContractorFactory factory(P, env);
      SharedDag dag = factory.getDag();

      ContractorHC4Revise h0(dag, 0);
      ContractorHC4Revise h1(dag, 1);

      Scope S( {x, y, z} );
      IntervalBox B(S);
      cout << "B =: " << B << endl;

      int NS = 10000;
      Timer tim;
      tim.start();
      for (int i=0; i<NS; ++i)
      {
         B.set(x, Interval(-2, 2));
         B.set(y, Interval(-2, 2));
         h1.contract(B);
      }
      tim.stop();
      cout << tim.elapsedTime() << " (s)" << endl;

      Constraint c1 = P.ctrAt(1);
      Timer tom;
      tom.start();
      for (int i=0; i<NS; ++i)
      {
         B.set(x, Interval(-2, 2));
         B.set(y, Interval(-2, 2));
         c1.contract(B);
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
