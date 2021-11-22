#include <iostream>
#include "realpaver_problem.hpp"
#include "realpaver_contractor_bc3.hpp"
#include "realpaver_contractor_bc4.hpp"
#include "realpaver_contractor_hc4.hpp"
#include "realpaver_dag.hpp"
#include "realpaver_thick_fun.hpp"
#include "realpaver_interval_slicer.hpp"
#include "realpaver_interval_union.hpp"
#include "realpaver_newton.hpp"
#include "realpaver_term_deriver.hpp"
#include "realpaver_term_fixer.hpp"
#include "realpaver_timer.hpp"
#include "realpaver_contractor_int.hpp"
#include "realpaver_scope.hpp"
#include "realpaver_model_bco.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::connectConsole();
   Interval::precision( 16 );

   try
   {
      Problem prob;

      Variable x = prob.addRealVar(1, 3, "x"),
               y = prob.addRealVar(-1, 5, "y");

      // prob.addObj( minimize(2*sqr(x) + 3*y - 1) );
      // BcoModel model(prob);
      
      Term t = sqr(x)*y - y - x + 1;

      prob.addCtr( t <= 0 );

      Dag dag;
      dag.insert( prob.ctrAt(0) );

      DagFun* f0 = dag.fun(0);

      Box B( prob.getBox() );

      cout << "B : " << B << endl;
      
      cout << "f0(B) : " << f0->eval(B) << endl;   
      f0->diff();      
      cout << "df0 : " << f0->grad() << endl;

/*
      Problem other;
      bool sat = prob.preprocess(other);

      if (sat)
         cout << endl << "-- Simplified problem --" << endl << other << endl;
      else
         cout << endl << "Problem unsatisfiable" << endl;
   
      Constraint cc( 3*x*2 == 1 );
      cout << cc << " --- " << cc.isLinear() << endl;
*/

      //~ cout << "occ : " << g.fun(0)->nbOcc(x.id()) << endl;;

      //~ Hc4Contractor hc4(&g, 0);
      //~ cout << hc4.contract(V) << " " << V << endl;

      //~ Bc3Contractor bc3(&g, 0, x.id());
      //~ cout << bc3.contract(V) << " " << V << endl;

      //~ Bc4Contractor bc4(&g, 0);
      //~ cout << bc4.contract(V) << " " << V << endl;

//      cout << f->evalOnly(y, Interval(0,1)) << endl;

      //~ cout << f->diff(V) << endl;
      //~ cout << f->deriv(0) << ", " << f->deriv(1) << ", " << f->deriv(2) << endl;
      //~ cout << f->deriv(x) << ", " << f->deriv(y) << ", " << f->deriv(z) << endl;

      //~ cout << f->diffOnly(z, Interval(0,2)) << endl;
      //~ cout << f->deriv(x) << ", " << f->deriv(y) << ", " << f->deriv(z) << endl;

      //~ DagThickFun ft(&g, 0, z);
      //~ cout << ft.update(V) << endl;
      
      //~ std::pair<Interval,Interval> res = ft.evalDiff(Interval(0,2));
      
      //~ cout << res.first << ", " << res.second << endl;


      //~ size_t i2 = g.insert( sqr(x) + 2 == 0 );

      //~ DagThickFun f2(&g, i2, x);
      //~ f2.update(V);

      //~ Interval x2(0, 100);

      //~ Newton o2(&f2);
      //~ o2.setPrecision(Precision::adaptive(1.0e-6));
      //~ o2.setMaxSteps(20);
      //~ cout << o2.localSearch(x2) << " " << x2 << endl;

      //~ IntervalUnion u = {Interval(3,4), Interval(1,2), Interval(5,6), Interval(0,3.5)};
      //~ cout << u << endl;
      
      //~ Term t1( sqr(x) + x*log(y) - maxi(x,y));
      //~ TermDeriver deriver(x);
      //~ t1.acceptVisitor(deriver);
      //~ cout << deriver.get() << endl;

      // size_t i2 = g.insert( sqr(x-3) - 2 == 0 );
      // fx : [-2, 2]  dx : [-4, 1]
      // c : 2.25  fc : <-1.4375, -1.4375>
      // ext div -> : [-inf, -1.4375] , [0.359375, inf]
      // maybe [1, 1.890625]
      // c - fc / dx : [-inf, 1.890625] , [3.6875, inf]

/*
      DagFun* f = g.fun(0);
      bool res = f->diff(V);
      cout << res << " " << f->grad() << endl;
*/
      //cout << f->sharedHc4Revise(V) << endl;
      //cout << V << endl;


/*
      Timer tim;
      tim.start();
      for (size_t i=0; i<100000; ++i)
      {
         f->hc4Revise(V);
      }
      tim.stop();
      cout << tim.elapsedTime() << " ms" << endl;
*/
   }
   catch(Menhir m)
   {
      cout << m.what() << endl;
   }

   return 0;
}
