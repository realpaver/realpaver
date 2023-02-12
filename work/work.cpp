#include <iostream>
#include <fstream>
#include "realpaver/Dag.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/PolytopeHullContractor.hpp"
#include "realpaver/Range.hpp"
#include "realpaver/VariableVector.hpp"
#include "realpaver/QuadraticTerm.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "work.log");
   Interval::precision( 12 );

   try {
      Problem prob;
      Variable x = prob.addRealVar(-3, 2, "x");
      Variable y = prob.addRealVar(-2, 4, "y");
      Variable z = prob.addRealVar(-1, 6, "z");
      Variable b = prob.addBinaryVar("b");
      Variable q = prob.addIntVar(0, 10, "q");
      
      cout << x.id() << " " << y.id() << endl;

      cout << y.getDomain() << ' ' << y.getTolerance() << ' ' << y.hashCode() << endl;



      cout << endl << endl << "#########################" << endl;
      Term tata = (-2*z) * z;
      cout << "tata := " << tata << endl;


      cout << endl << endl;

      Term tt (  (2*sqr(y) - (3 + x*y)) + y*z - y - 5*sqr(z));
      
      cout << "tt : " << tt << endl;
      cout << endl << endl;
      
      
//      Term tt = 2*sqr(x) + 1.0 - y + 2*x*y + 3*y*x - sqr(y);
      QuadraticTerm qt(tt);

      cout << "qt : " << qt << endl;
      cout << "to term : " << qt.toTerm() << endl;

      Term qtf = qt.factorize();
      cout << "factorized : " << qtf << endl;
      
      cout << endl << "---------------------------------" << endl;

      IntervalRegion XX = prob.getDomains();
      cout << "tt in " << tt.eval(XX) << endl;
      cout << "qtf in " << qtf.eval(XX) << endl;

      cout << endl << "---------------------------------" << endl;

      
      Scope S = prob.scope();
      cout << S << endl;
   
      for (const auto& v : S)
         cout << v.getName() << ' ';
      cout << endl;
      
      IntervalRegion reg = prob.getDomains();
      cout << reg << endl
           << "b in " << reg.get(b) << endl << b << endl << b.getTolerance() << endl;
           
           
           reg.set(x, Interval(0, 0.51));

   x.setTolerance(Tolerance::makeAbs(0.5));
   y.setTolerance(Tolerance::makeRel(0.25));

      cout << "GRID PERI : " << reg.gridPerimeter() << endl;
           
           
           
return 0;

      Term t( y - sqr(x) );
      cout << "t = " << t << endl
           << t.eval(reg) << endl
           << t.contract(reg, Interval(0, 1)) << endl
           << reg.subRegion(t.scope()) << endl;

      Constraint c( t == 100 );

      cout << "c: " << c         << endl;
      
      if (c.isSatisfied(reg) != Proof::Empty) {
         Proof certificate = c.contract(reg);
         cout << certificate << ' ' << reg << endl;
      }
      else
         cout << c.violation(reg)   << endl;

      SharedDag dag = std::make_shared<Dag>();
      size_t i1 = dag->insert( y - sqr(x) == 0);
      size_t i2 = dag->insert( sqr(x) + sqr(y) - 2 == 0);

      cout << (*dag) << endl;

      cout << "input reg " << reg << endl;

      PolytopeHullContractor phc(dag, PolytopeCreatorStyle::Taylor);
      Proof proof = phc.contract(reg);

cout << proof << ' ' << reg.subRegion(phc.scope()) << endl;

/*
      double x1 = 2,   y1 = 1.5, d1 = 2.55,
             x2 = 6,   y2 = 5.5, d2 = 3.81,
             x3 = 8.5, y3 = 1,   d3 = 4.12,
             E = 1.0e-2;


      Problem prob;
      Variable xM = prob.addRealVar(-1.0e8, 1.0e8, "xM");
      Variable yM = prob.addRealVar(-1.0e8, 1.0e8, "yM");

      prob.addCtr({ in(dist(xM, yM, x1, y1), Interval(d1-E, d1+E)),
                    in(dist(xM, yM, x2, y2), Interval(d2-E, d2+E)),
                    in(dist(xM, yM, x3, y3), Interval(d3-E, d3+E)) });

      NcspSolver solver(prob);

      Param prm;
      prm.loadParam("ParamFile");
      solver.getEnv()->setParam(prm);

      solver.solve();
*/
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
