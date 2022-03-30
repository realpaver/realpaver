#include <fstream>
#include <iomanip>
#include <iostream>
#include "realpaver/BOSolver.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Parser.hpp"

using namespace realpaver;
using namespace std;

size_t lengthVarName(const Problem& p);
string logFilename(const std::string& filename);
bool processArgs(int argc, char** argv, string& filename, string& pfilename);

// ./bop_solver Beale.bop -p bop_settings
int main(int argc, char** argv)
{
   try {
      Problem problem;
      Parser parser;
      string filename = "", pfilename = "";

      // processes the arguments
      bool ok = processArgs(argc, argv, filename, pfilename);
      if (!ok) THROW("Bad arguments on the command line");

      // tries to open the problem file
      ifstream infile(filename);
      if (!infile.is_open()) THROW("Bad problem filename");
      else infile.close();

      // reads the parameters
      Param prm;
      if (pfilename != "") prm.loadParam(pfilename);

      // logger
      LogLevel loglevel = StringToLogLevel(prm.getStrParam("LOG_LEVEL"));
      if (loglevel != LogLevel::none)
      {
         string log = logFilename(filename);
         Logger::init(loglevel, log);
      }

      LOG_MAIN("Bound-constrained optimization");
      LOG_MAIN("Input file > " << filename);

      // parsing
      ok = parser.parseFile(filename, problem);
      if (!ok) THROW("Parse error: " << parser.getParseError());
      if (!problem.isBOP())
         THROW("Not a box-constrained optimization problem");

      // solving
      BOSolver solver(problem);
      solver.setParam(prm);

      int prec = prm.getIntParam("FLOAT_PRECISION");
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
         size_t lmax = std::max(lengthVarName(problem), objname.size());

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

size_t lengthVarName(const Problem& p)
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

bool processArgs(int argc, char** argv, string& filename, string& pfilename)
{
   bool hasfile = false;
   int i = 1;
   
   while (i<argc)
   {
      std::string text(argv[i]);
      if (text[0] != '-' && !hasfile)
      {
         filename = text;
         hasfile = true;
         i = i+1;
      }
      else if (text == "-p")
      {
         i = i+1;
         if (i == argc) return false;
         else
         {
            pfilename = std::string(argv[i]);
            i = i+1;
         }
      }
      else return false;
   }

   return hasfile;
}

string logFilename(const std::string& filename)
{
   string log = "";
   if (filename[0] == '.') return log;

   bool hasPoint = false;

   // finds an extension if any
   int i = filename.length() - 1;
   while (i>=0 and !hasPoint)
   {
      if (filename[i] == '.')
      {
         hasPoint = true;
         log = filename.substr(0, i);
      }
      i = i-1;
   }

   if (!hasPoint) log = filename;

   return log + ".log";
}
