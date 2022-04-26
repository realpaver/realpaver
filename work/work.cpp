#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/BOModel.hpp"

#include "realpaver/PolytopeHullContractor.hpp"
#include "realpaver/RealMatrix.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 4 );

   RealMatrix A = {{1, 2}, {3, 4}, {5, 6}};
   cout << "A:" << endl << A << endl;

   RealMatrix B = {{1, 0, 1, 0}, {3, 0, 3, 0}};
   cout << "B:" << endl << B << endl;

   cout << "A*B:" << endl << A*B << endl;
   
   
   try {
      Problem* prob = new Problem;
      //~ Variable ksi_1 = prob->addRealVar(-1.199700000000001, -0.8532, "ksi_1"),
               //~ ksi_2 = prob->addRealVar(1, 5, "ksi_2"),
               //~ ksi_3 = prob->addRealVar(3.6, 9.800000000000001, "ksi_3"),
               //~ ksi_4 = prob->addRealVar(-26, -9.54, "ksi_4"),
               //~ R_C  = prob->addRealVar(0.1, 0.8000000000000001, "R_C"),
               //~ lambda = prob->addRealVar(10, 23, "lambda"),
               //~ beta = prob->addRealVar(0.0136, 0.5, "beta"),




Variable x = prob->addRealVar(2, 6, "x"),
         y = prob->addRealVar(1, 3, "y"),
         z = prob->addRealVar(0, 10, "z");
   
Constraint c(z >= sqr(y) + 2*x);
IntervalRegion Z = prob->getDomains();
cout << c.contract(Z) << endl;
cout << Z << endl;



return 0;







      Variable ksi_1 = prob->addRealVar(-1.1997, -1.0, "ksi_1"),
               ksi_2 = prob->addRealVar(3, 4, "ksi_2"),
               ksi_3 = prob->addRealVar(5, 6, "ksi_3"),
               ksi_4 = prob->addRealVar(-18, -16, "ksi_4"),
               R_C  = prob->addRealVar(0.4, 0.6, "R_C"),
               lambda = prob->addRealVar(21, 22, "lambda"),
               beta = prob->addRealVar(0.0136, 0.2, "beta"),
   //~ ksi_1  = -1.187379966183
   //~ ksi_2  = 3.552344524501
   //~ ksi_3  = 5.697645819184
   //~ ksi_4  = -17.384224912130
   //~ R_C    = 0.510157282767
   //~ lambda = 21.476561754537
   //~ beta   = 0.014886955676
    
      // introduction of a new variable
      // the square is equal to v^2
      v = prob->addRealVar(-23.80055422214025, 36.59251400794607, "v"),
      sv = prob->addRealVar(0, Double::inf(), "sv");

      IntervalRegion reg = prob->getDomains();
      Scope sco = prob->scope();

      Interval I1(0.338151,0.338151),
               I2(-0.0472331,-0.0472331),
               I3(0.00813909,0.00813909),
               I4(3.2994,3.2994),
               I5(1.5442,1.5442),
               I6(-1.86733,-1.86733),
               I7(-0.650156,-0.650156),
               I8(1.195,1.19501),
               I9(24*11.1001*I4/(27*I5)),
               J1(24*I1);
   
      //~ Term t = 17.8001 -
        //~ 24 *
        //~ (  ksi_1 + I1*ksi_2 + I2*ksi_3 + I3*ksi_4 -
            
           //~ 11.1001 * (
                 //~ 0.00100001*R_C + (I4/(I5*(lambda+I6)))/27
           //~ ) +

           //~ I7*beta+I8
        //~ )
      //~ ;

      Term t = 17.8001 - 24*ksi_1 - J1.midpoint()*ksi_2 - (24*I2)*ksi_3
               - (24*I3)*ksi_4 +

           //~ 24*11.1001 * (
                 //~ 0.00100001*R_C + (I4/(I5*(lambda+I6)))/27
           //~ )
           
           
           (24*11.1001*0.00100001)*R_C +
           
           I9/(lambda + I6)

           - (24*I7)*beta - (24*I8)
      ;

      Term sqt = sqr(t);

      cout << t << endl;
      cout << "t in " << t.eval(reg) << endl;
      cout << "sqt in " << sqt.eval(reg) << endl;

      Dag* dag = new Dag();

      size_t i = dag->insert(v == t);
      DagFun* ft = dag->fun(i);

      size_t j = dag->insert(sv == sqr(v));
      DagFun* fst = dag->fun(j);

      cout << endl;
      cout << "input reg = " << reg << endl << endl;

      // split domain of v (t)
      IntervalRegion X(reg);
      X.set(v, Interval(-0.1, 0.1));

      cout << "split dom v (t) : " << X.get(v) << endl;
      X.set(sv, sqr(X.get(v)));
      
      cout << "assign dom sv (t) : " << X.get(sv) << endl;

      ////////////////////////////////////////////////////////////////
      cout << endl << "reg before hc4 : " << X << endl;

      ////////////////////////////////////////////////////////////////
      Proof proof = fst->hc4Revise(X);

      cout << endl << "-- CONTRACTION / sv = v^2 -> ";
      cout << proof << endl;
      cout << "reg = " << X << endl;

      ////////////////////////////////////////////////////////////////
      proof = ft->hc4Revise(X);

      cout << endl << "-- CONTRACTION / v = t -> ";
      cout << proof << endl;
      cout << "reg = " << X << endl;

      ////////////////////////////////////////////////////////////////
      cout << endl << "-- DIFFERENCE" << endl;
      for (auto v : sco)
      {
         Interval I(reg.get(v)), J(X.get(v));
         if (I.isSetNeq(J))
            cout << v.getName() << " : " << I << " -> " << J << endl;
      }

      // t in : [-23.80055422214025, 36.59251400794607]
      // t^2 in [-0, 1339.01208142173]

   PolytopeHullContractor ph(dag, sco);
   proof = ph.contract(X);
      cout << proof << endl;
      cout << "reg = " << X << endl;

   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
