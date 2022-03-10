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

      //~ Beale
      //~ Variable x = problem.addRealVar(-4.5,  4.5, "x"),
               //~ y = problem.addRealVar(-4.5,  4.5, "y");
      //~ Term to = sqr(1.5-x+x*y) + sqr(2.25-x+x*sqr(y)) + sqr(2.625-x+x*pow(y,3));


      //~ Colville
      //~ Variable x1 = problem.addRealVar(-10,  10, "x1"),
               //~ x2 = problem.addRealVar(-10,  10, "x2"),
               //~ x3 = problem.addRealVar(-10,  10, "x3"),
               //~ x4 = problem.addRealVar(-10,  10, "x4");
      //~ Term to = 100.0*sqr(sqr(x1)-x2) + sqr(x1-1.0) + sqr(x3-1.0) +
                //~ 90.0*sqr(sqr(x3)-x4) + 10.1*(sqr(x2-1.0) + sqr(x4-1.0)) +
                //~ 19.8*(x2-1.0)*(x4-1.0);

      Variable x = problem.addRealVar(-10,  10, "x"),
               y = problem.addRealVar(-10,  10, "y");
      Term to = sqr(x + 2*y - 7) + sqr(2*x + y - 5);

      //~ Variable x = problem.addRealVar(-10,  10, "x"),
               //~ y = problem.addRealVar(-10,  10, "y");
      //~ Term to = sqr(x + y);


      problem.addObjective(minimize(to));

      BOSolver solver(problem);
      solver.setNodeLimit(10);

      solver.optimize();
      OptimizationStatus status = solver.getStatus();

      std::string sep = "##################################################";
      std::string indent = "   ";

      cout << sep << endl;
      cout << std::setprecision(prec)
           << indent << "Preprocessing time.......... "
           << solver.getPreprocessingTime() << " (s)"
           << endl;
      
      if (solver.getNbNodes() > 0)
      {
         cout << indent << "Solving time................ "
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
                 << solver.getObjTol() << endl;
         }
         else
         {
            cout << indent << "GLOBAL OPTIMUM NOT FOUND " << endl;

            if (status == OptimizationStatus::StopOnTimeLimit)
            {
               cout << indent << "TIME LIMIT REACHED: "
                    << solver.getTimeLimit() << endl;
            }
            
            if (status == OptimizationStatus::StopOnNodeLimit)
            {
               cout << indent << "NODE LIMIT REACHED: "
                    << solver.getNodeLimit() << endl;
            }
         }
   
         RealVector sol = solver.getBestSolution();

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
            
            cout << " = " << sol[v.getId()]
                 << endl;
         }
      }
      cout << sep << endl;
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
