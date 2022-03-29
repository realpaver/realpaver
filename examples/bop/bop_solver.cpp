#include <iomanip>
#include <iostream>
#include "realpaver/BOSolver.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Parser.hpp"

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
   Logger::init(LogLevel::internal, "bop_solver.log");

   try {
      //~ string filename = "Beale.bop";
      //~ string filename = "Colville.bop";
      //~ string filename = "SixHumpCamel.bop";
      string filename = "ThreeHumpCamel.bop";

      Problem problem;
      Parser parser;

      bool res = parser.parseFile(filename, problem);

      if (!res)
         THROW("Parse error: " << parser.getParseError());

      if (!problem.isBOP())
         THROW("Not a box-constrained optimization problem");

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
