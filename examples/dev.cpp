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
#include "realpaver/ConstraintFixer.hpp"

using namespace realpaver;
using namespace std;


// meson setup build -DLP_LIB=Highs -DASSERT=true -DLOG=true -Dbuildtype=debug
// meson setup build -DLP_LIB=Highs -DASSERT=false -DLOG=false -Dbuildtype=release

int main(void)
{
   try
   {
      Problem P;
      Variable a1 = P.addRealVar(-0.854470265157, -0.776666666666, "a1"),
               a2 = P.addRealVar(-0.0300000000001, -0.00999999999999, "a2"),
               s5 = P.addRealVar(6.89999999999, 7, "s5"),
               s4 = P.addRealVar(1.34999999999, 1.45000000001, "s4");

      Constraint c( (-89760*(-1 - s4))/(1 - a1 + a2) +
                    (7312635*(1 - s4))/(32*(1 + a1 + a2)) +
                    (7677115*s4)/32 - (99586*(-s4 + s5))/(1 + a1*s5 + a2*sqr(s5))
                    == 0 );

      Term t1( (-89760*(-1 - s4))/(1 - a1 + a2) ),
           t2( (7312635*(1 - s4))/(32*(1 + a1 + a2)) ),
           t3( (7677115*s4)/32 - (99586*(-s4 + s5))/(1 + a1*s5 + a2*sqr(s5)) );

      Term tc( t1 + t2 + t3);
   
      Term tt( t1 + t2);

      IntervalBox B( P.scope() );

      cout << "c : " << c << endl;
      cout << c.isSatisfied(B) << endl;


      cout << "t1 : " << t1 << endl;
      cout << t1.eval(B) << endl;

      cout << "t2 : " << t2 << endl;
      cout << t2.eval(B) << endl;

      cout << "t3 : " << t3 << endl;
      cout << t3.eval(B) << endl;



      cout << "tt : " << tt << endl;
      cout << tt.eval(B) << endl;

      SharedDag dag = std::make_shared<Dag>();
      dag->insert( t1 + t2 == 0 );
      DagFun* f = dag->fun(0);

      cout << f->intervalEval(B) << endl;;


      cout << (*dag) << endl;
      
      dag->printIntervalValues(std::cout);
/*
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
      */

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
