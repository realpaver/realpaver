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
      Param::init("../src/realpaver/settings.txt");
      Param::print(cout);
   }
   catch (Exception ex) {
      cout << ex.what() << endl;
   }

   int prec = Param::getIntParam("FLOAT_PRECISION");
   Interval::precision(prec);

   try {
      Problem problem;

      Variable x = problem.addRealVar(-4.5,  4.5, "x"),
               y = problem.addRealVar(-4.5,  4.5, "y");

      Term to = sqr(1.5-x+x*y) + sqr(2.25-x+x*sqr(y)) + sqr(2.625-x+x*pow(y,3));

      problem.addObjective(minimize(to));

      BOSolver solver(problem);

      solver.optimize();
      OptimizationStatus status = solver.getStatus();

      cout << "----------------------------------------" << endl;
      cout << std::setprecision(prec)
           << "Preprocessing time: " << solver.getPreprocessingTime() << " (s)"
           << endl
           << "Solving time:       " << solver.getSolvingTime() << " (s)"
           << endl
           << "Number of nodes:    " << solver.getNbNodes() << endl
           << "----------------------------------------"
           << endl;

      if (status == OptimizationStatus::Infeasible)
      {
         cout << "PROBLEM INFEASIBLE" << endl;
      }
      else
      {
         if (status == OptimizationStatus::Optimal)
         {
            cout << "GLOBAL OPTIMUM FOUND | " << solver.getObjTol() << endl;
         }
         else
         {
            cout << "GLOBAL OPTIMUM NOT FOUND " << endl;

            if (status == OptimizationStatus::StopOnTimeLimit)
            {
               cout << "TIME LIMIT REACHED: " << solver.getTimeLimit() << endl;
            }
            
            if (status == OptimizationStatus::StopOnNodeLimit)
            {
               cout << "NODE LIMIT REACHED: " << solver.getNodeLimit() << endl;
            }
         }
   
         RealVector sol = solver.getBestSolution();

         std::string objname = "obj";
         size_t lmax = std::max(maxSizeVarName(problem), objname.size());

         cout << objname;
         for (size_t j=objname.size(); j<lmax; ++j) cout << " ";
         cout << " = " << solver.getObjEnclosure() << endl;

         for (size_t i=0; i<problem.nbVars(); ++i)
         {
            Variable v = problem.varAt(i);
            cout << v.getName();

            for (size_t j=v.getName().size(); j<lmax; ++j) cout << " ";
            
            cout << " = " << sol[v.getId()]
                 << endl;
         }
      }
      cout << "----------------------------------------" << endl;
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
