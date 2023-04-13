#include <iostream>
#include <fstream>
#include <iomanip>
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
#include "realpaver/HC4Contractor.hpp"

using namespace std;
using namespace realpaver;

int main(void)
{
   Logger::init(LogLevel::full, "complex.log");

   try {
      Problem P;
      Variable r = P.addRealVar(0, 10, "r");
      Variable t = P.addRealVar(Interval::zeroTwoPi(), "t");

      r.setTolerance(Tolerance::makeAbs(0.05));
      t.setTolerance(Tolerance::makeAbs(0.05));

      bool iftc = true;

      Term tc = -42 + r*(10*cos(t) + r*(177.5*cos(2*t) + r*(221*cos(3*t) +
                r*(107*cos(4*t) + r*(29.5*cos(5*t) + r*(3*cos(6*t))))))),
           ts = r*(10*sin(t) + r*(177.5*sin(2*t) + r*(221*sin(3*t) +
                r*(107*sin(4*t) + r*(29.5*sin(5*t) + r*(3*sin(6*t)))))));

      if (iftc)
         P.addCtr(tc == 0);
      else
         P.addCtr(ts == 0);

      Param prm;
      prm.setIntParam("SOLUTION_LIMIT", 1000000);

      NcspSolver solver(P);
      solver.getEnv()->setParam(prm);
      
      solver.solve();

      string filename = "complex.out";

      ofstream of;
      of.open(filename, ofstream::out);

      size_t nsol = solver.getNbSolutions();

      if (iftc)
         of << "r = [";
      else
         of << "rr = [";

      for (size_t i=0; i<nsol; ++i)
      {
         std::pair<IntervalRegion, Proof> sol = solver.getSolution(i);
         double rval = sol.first.get(r).midpoint();

         double z = std::abs(rval);
         if (z < 0.001) of << 0;
         else
            of << std::setprecision(4) << rval;

         if (i<nsol-1) of << ", ";
         
         if ((i+1) % 10 == 0) of << endl;
      }
      of << "]" << endl << endl;

      if (iftc)
         of << "t = [";
      else
         of << "tt = [";

      for (size_t i=0; i<nsol; ++i)
      {
         std::pair<IntervalRegion, Proof> sol = solver.getSolution(i);
         double tval = sol.first.get(t).midpoint();

         double z = std::abs(tval);
         if (z < 0.001) of << 0;
         else
            of << std::setprecision(4) << tval;

         if (i<nsol-1) of << ", ";

         if ((i+1) % 10 == 0) of << endl;
      }
      of << "]" << endl;
      of.close();

      return 0;
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   return 0;
}
