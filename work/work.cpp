#include <iostream>
#include "realpaver/Problem.hpp"
#include "realpaver/contractor_bc3.hpp"
#include "realpaver/contractor_bc4.hpp"
#include "realpaver/contractor_hc4.hpp"
#include "realpaver/dag.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/thick_fun.hpp"
#include "realpaver/interval_slicer.hpp"
#include "realpaver/interval_union.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/term_deriver.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Timer.hpp"
#include "realpaver/contractor_int.hpp"
#include "realpaver/scope.hpp"
#include "realpaver/bco_model.hpp"
#include "realpaver/IntervalVector.hpp"
#include "realpaver/Param.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::internal, "work.log");
   Interval::precision( 16 );

   try {
      Param::init("../src/realpaver/settings.txt");
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }


   //Problem p;
      //~ Variable x = problem->addRealVar(0, 10, "x"),
               //~ y = problem->addRealVar(-2, 6, "y"),
               //~ z = problem->addRealVar(0, 10, "z");

      //~ problem->addObjective(minimize(sqr(x) + pow(y,3) - x*y - 2*z));

   try
   {
      Problem* problem = new Problem();
      Problem* simpl = nullptr;

      Variable x = problem->addRealVar(0, 1, "x"),
               y = problem->addRealVar(1, 1, "y"),
               z = problem->addRealVar(-3, -3, "z");

      problem->addObjective(minimize(sqr(x) + pow(y,3) - x*y - 2*z));
      
      problem->addCtr( x + y + z >= 0 );

      cout << "-- PROBLEM --\n" << *problem << endl;

      Preprocessor prepro;
      simpl = new Problem();

      prepro.apply(*problem, *simpl);
      cout << "-- AFTER PREPRO --\n" << *simpl << endl;

      //~ BcoModel* model = new BcoModel(*prob);
      //~ BcoResult res = model->preprocess();

      //~ LOG_INTERNAL("after preprocess");

      //~ if (res.getProof() != Proof::Empty /*&& res.getProof() != Proof::Optimal */)
      //~ {
         //~ IntervalVector* B = res.getBox();
         //~ Interval lu = B->at(model->objVar().getId());

         //~ if (model->nbFixedVars() > 0)
         //~ {
            //~ simpl = new Problem();

            //~ // fixes the variable representing the objective function in order to
            //~ // remove it from the simplified problem
            //~ B->set(model->objVar().getId(), Interval::zero());

            //~ // creates the simplified problem
            //~ prob->preprocess(*B, *simpl);

            //~ // creates the model from the simplified problem
            //~ delete model;
            //~ model = new BcoModel(*simpl);
         //~ }

         //~ // assigns the domain of the objective variable
         //~ model->setObjDomain(lu);

         //~ // branch-and-bound to solve the model
         //~ res = model->solve();
      //~ }

      //~ LOG_INTERNAL("proof : " << res.getProof());

      delete problem;
      if (simpl != nullptr) delete simpl;
  }
   
/*
      Box B( prob.getBox() );

      Term f( sqr(x) -2*y + x*z );
      cout << "min. " << f << endl;
      cout << "s.t. " << B << endl << endl;
      
      TermDeriver dx( x );
      f.acceptVisitor( dx );
      TermDeriver dy( y );
      f.acceptVisitor( dy );
      TermDeriver dz( z );
      f.acceptVisitor( dz );

      Term df_dx( dx.get() );
      Term df_dy( dy.get() );
      Term df_dz( dz.get() );

      Interval e_f( f.eval(B) );
      Interval e_df_dx( df_dx.eval(B) );
      Interval e_df_dy( df_dy.eval(B) );
      Interval e_df_dz( df_dz.eval(B) );

      cout << "e_df_dx : " << e_df_dx << endl;
      cout << "e_df_dy : " << e_df_dy << endl;
      cout << "e_df_dz : " << e_df_dz << endl << endl;

      double val_y = B[y].right();
      cout << "Fix y : " << val_y << endl;
      B.set(y, val_y);
      cout << "B : " << B << endl << endl;

      Dag dag;
      size_t ix = dag.insert( df_dx == 0 );
      size_t iz = dag.insert( df_dz == 0 );

      Bc4Contractor op_x(&dag, ix);
      Bc4Contractor op_z(&dag, iz);

      Box BB(B);

      Proof p_z = op_z.contract(BB);
      cout << "p_z : " << p_z << " -> " << BB[z] << endl;

      double val_z = B[z].right();
      cout << "Fix z : " << val_z << endl;
      B.set(z, val_z);
      cout << "B : " << B << endl << endl;

      Proof p_x = op_x.contract(BB);
      cout << "p_x : " << p_x << " -> " << BB[x] << endl;
      cout << "Remove the left bound x=-8 since df_dx < 0" << endl << endl;

      cout << "New problem: min x^2 + 10x - 10 with -5 <= x <= -1" << endl << endl;

      Interval ee_f = sqr(BB[x] + 5) - 35;
      cout << "f = (x+5)^2 -35 -> " << ee_f << endl << endl;
      cout << "min f = " << ee_f.left() << endl << endl;
*/

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

   catch(Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
