#include <iomanip>
#include <iostream>
#include "realpaver/BOSolver.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

using namespace realpaver;
using namespace std;

size_t maxSizeVarName(const Problem& p)
{
   size_t res = 0;

   for (size_t i=0; i<p.nbVars(); ++i)
   {
      Variable v = p.varAt(i);
      size_t n = v.getName().size();
      if (n > res) res = n;
   }
   return res;
}

int main(void)
{
   Logger::init(LogLevel::internal, "bop.log");

   try {
      Problem problem;

      //~ Beale
      //~ Variable x = problem.addRealVar(-4.5,  4.5, "x"),
               //~ y = problem.addRealVar(-4.5,  4.5, "y");
      //~ Term to = sqr(1.5-x+x*y) + sqr(2.25-x+x*sqr(y)) + sqr(2.625-x+x*pow(y,3));

      // 3-hump camel
      //~ Variable x = problem.addRealVar(-5,  5, "x"),
               //~ y = problem.addRealVar(-5,  5, "y");
      //~ Term to = 2.0*sqr(x) - 1.05*pow(x, 4) + pow(x, 6)/6.0 + x*y + sqr(y);

      // 6-hump camel
      Variable x = problem.addRealVar(-3,  3, "x"),
               y = problem.addRealVar(-2,  2, "y");
      Term to = (4.0-2.1*sqr(x)+pow(x,4)/3.0)*sqr(x) + x*y +
                (4.0*sqr(y)-4.0)*sqr(y);

      // Colville
      //~ Variable x1 = problem.addRealVar(-10,  10, "x1"),
               //~ x2 = problem.addRealVar(-10,  10, "x2"),
               //~ x3 = problem.addRealVar(-10,  10, "x3"),
               //~ x4 = problem.addRealVar(-10,  10, "x4");
      //~ Term to = 100.0*sqr(sqr(x1)-x2) + sqr(x1-1.0) + sqr(x3-1.0) +
                //~ 90.0*sqr(sqr(x3)-x4) + 10.1*(sqr(x2-1.0) + sqr(x4-1.0)) +
                //~ 19.8*(x2-1.0)*(x4-1.0);

      //~ Variable x = problem.addRealVar(-10,  10, "x"),
               //~ y = problem.addRealVar(-10,  10, "y");
      //~ Term to = sqr(x + 2*y - 7) + sqr(2*x + y - 5);


//      problem.addObjective(MAX(to));
      problem.addObjective(MIN(to));

      BOSolver solver(problem);
      solver.setIntParam("NODE_LIMIT", 400);
      solver.setIntParam("BB_SPACE_FREQUENCY", 2);
      solver.setStrParam("SPLIT_OBJECTIVE", "NO");
      solver.setStrParam("LOCAL_SOLVER_ALGORITHM", "GRADIENT");
      solver.setStrParam("PROPAGATION_ALGORITHM", "MAX_CID_HC4");
      solver.setStrParam("SPLIT_SELECTOR", "ROUND_ROBIN");

      int prec = solver.getIntParam("FLOAT_PRECISION");
      Interval::precision(prec);

      solver.optimize();
      OptimizationStatus status = solver.getStatus();

      std::string sep = "##################################################";
      std::string indent = "   ";

      cout << sep << endl;
      cout << std::fixed << std::setprecision(2)
           << indent << "Preprocessing time.......... "
           << solver.getPreprocessingTime() << " (s)"
           << endl;

      if (solver.getNbNodes() > 0)
      {
         cout << indent << "Solving time................ "
              << std::fixed << std::setprecision(2)
              << solver.getSolvingTime() << " (s)"
              << endl
              << indent << "Number of nodes............. "
              << solver.getNbNodes() << endl;
      }
      
      cout << sep << endl;

      if (status == OptimizationStatus::Infeasible)
      {
         cout << indent << "PROBLEM INFEASIBLE" << endl;
      }
      else
      {
         if (status == OptimizationStatus::Optimal)
         {
            cout << indent << "GLOBAL OPTIMUM FOUND | "
                 << solver.getTolParam("OBJ_TOL") << endl;
         }
         else
         {
            cout << indent << "GLOBAL OPTIMUM NOT FOUND " << endl;

            if (status == OptimizationStatus::StopOnTimeLimit)
            {
               cout << indent << "TIME LIMIT REACHED: "
                    << solver.getDblParam("TIME_LIMIT") << "(s)" << endl;
            }
            
            if (status == OptimizationStatus::StopOnNodeLimit)
            {
               cout << indent << "NODE LIMIT REACHED: "
                    << solver.getIntParam("NODE_LIMIT") << endl;
            }
         }

         cout << std::scientific << std::setprecision(prec);
   
         RealPoint sol = solver.getBestSolution();

         std::string objname = "obj";
         size_t lmax = std::max(maxSizeVarName(problem), objname.size());

         cout << indent << objname;
         for (size_t j=objname.size(); j<lmax; ++j) cout << " ";
         
         Interval z = solver.getObjEnclosure();
         if (z.isSingleton())
            cout << " = " << z.left() << endl;
         else
            cout << " = " << z << endl;

         for (size_t i=0; i<problem.nbVars(); ++i)
         {
            Variable v = problem.varAt(i);
            cout <<  indent << v.getName();

            for (size_t j=v.getName().size(); j<lmax; ++j) cout << " ";

            cout << " = " << sol.get(v) << endl;
         }
      }
      cout << sep << endl;
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
