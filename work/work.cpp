#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/BOLocalConjugate.hpp"
#include "realpaver/BOSplit.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::internal, "work.log");
   Interval::precision( 16 );

   try {
      Problem P;
      Variable x = P.addRealVar(-4, 6, "x");
      Variable y = P.addRealVar(-3, 8, "y");
      Variable z = P.addRealVar(-10, 10, "z");
      Variable u = P.addRealVar(-1, 4, "u");
      Variable v = P.addRealVar(0, 9, "v");

      z.setTolerance(Tolerance::makeAbs(30.0));

      IntervalRegion initRegion = P.getDomains();


      RealPoint mp = initRegion.midpoint();
      cout << mp << endl;


      Dag D;
      size_t i = D.insert( sqr(y) - 3.0*u );

      DagFun* fi = D.fun(i);



      Scope S = {x, y, u, z};
//      Selector* selector = new SelectorMaxDom(S);
      Selector* selector = new SelectorMaxSmear(fi, S);
      IntervalSlicer* slicer = new IntervalPeeler(10.0);
      BOSplit split(selector, slicer);

      SharedBONode node = std::make_shared<BONode>(S, z, initRegion);

      bool res = split.apply(node);
      if (res)
      {
         int i = 0;
         auto it = split.begin();
         while (it != split.end())
         {
            IntervalVector* X = (*it)->getRegion();
            cout << ++i << ": " << (*X) << endl;
            ++it;
         }
      }
      else
      {
         cout << "NO SPLIT" << endl;
      }
      
/*
      RealVector initPoint = initRegion.midpoint();
      RealVector finalPoint(initPoint.size());

      Dag D;
      size_t i = D.insert( 1.5*sqr(x) + 3.0*sqr(y) + 2.0*x*y -2.0*x + 8.0*y );

      DagFun* fi = D.fun(i);

      BOLocalConjugate solver;

      OptimizationStatus status = solver.minimize(*fi, initRegion, initPoint, finalPoint);

      cout << status << endl << finalPoint << endl;
*/
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }


   return 0;
}
