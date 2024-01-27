#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "realpaver/config.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Stat.hpp"

using namespace realpaver;
using namespace std;

#define BLACK(s)   "\033[30m" << s << "\033[39m"
#define RED(s)     "\033[31m" << s << "\033[39m"
#define GREEN(s)   "\033[32m" << s << "\033[39m"
#define ORANGE(s)  "\033[33m" << s << "\033[39m"
#define BLUE(s)    "\033[34m" << s << "\033[39m"
#define MAGENTA(s) "\033[35m" << s << "\033[39m"
#define CYAN(s)    "\033[36m" << s << "\033[39m"
#define GRAY(s)    "\033[37m" << s << "\033[39m"

// parses the filename
void parseFilename(const std::string& filename, std::string& path,
                   std::string& base, std::string& ext);

// processes the argulments on the command line
bool processArgs(int argc, char** argv, string& filename, string& pfilename);

// inserts points at the end of a string
string WP(const string& s, int n);

// main function
int main(int argc, char** argv)
{
   try {
      string filename = "", pfilename = "", sfilename = "";

      // processes the arguments
      bool ok = processArgs(argc, argv, filename, pfilename);
      if (!ok) THROW("Bad arguments on the command line");
   
      // parses the problem file name
      string baseFilename, pathFilename, extFilename;
      parseFilename(filename, pathFilename, baseFilename, extFilename);
   
      // tries to open the problem file
      ifstream infile(filename);
      if (!infile.is_open()) THROW("Bad problem filename: " << filename);
      else infile.close();

      // reads the parameters
      Param prm;
      if (pfilename != "") prm.loadParam(pfilename);

      // logger
#if LOG_ON
      LogLevel loglevel = StringToLogLevel(prm.getStrParam("LOG_LEVEL"));
      string flog = "";
      if (loglevel != LogLevel::none)
      {
         flog = baseFilename + ".log";
         Logger::init(loglevel, flog);
      }
#endif

      LOG_MAIN("NCSP solving");
      LOG_MAIN("Input file: " << filename);

      // parsing
      Parser parser(prm);
      Problem problem;

      ok = parser.parseFile(filename, problem);
      if (!ok) THROW("Parse error: " << parser.getParseError());
      if (!problem.isCSP()) THROW("Not a NCSP");

      // solving
      NcspSolver solver(problem);
      solver.getEnv()->setParam(prm);

      std::string sep = "########################################";
      sep += sep;
      std::string indent = "   ";
      int wpl = 36;

      cout << GRAY(sep) << endl;
      cout << BLUE(REALPAVER_STRING) << BLUE(" NCSP solver") << endl;

      ////////////////////
      solver.solve();
      ////////////////////

      string solfilename = baseFilename + ".sol";
      ofstream fsol;
      fsol.open(solfilename, std::ofstream::out);
      if (fsol.bad()) THROW("Open error of solution file");

      // preliminaries
      cout << GRAY(sep) << endl;
      cout << BLUE("Files") << endl;

      cout << indent << WP("Input file", wpl) << BLACK(filename) << endl;

#if LOG_ON
      cout << indent << WP("Log file", wpl);
      string meslog = (loglevel != LogLevel::none) ? flog : "no log";
      cout << BLACK(meslog) << endl;
#endif

      cout << indent << WP("Output file", wpl) << BLACK(solfilename) << endl;

      // preprocessing
      cout << GRAY(sep) << endl;
      string prepro = solver.getEnv()->getParam()->getStrParam("PREPROCESSING");

      Preprocessor* preproc = solver.getPreprocessor();

      fsol << WP("NCSP solver", wpl) << REALPAVER_STRING << endl;

      auto now = chrono::system_clock::now();
      std::time_t end_time = chrono::system_clock::to_time_t(now);

      fsol << WP("Input file", wpl) << filename << endl;
      fsol << WP("Current date and time", wpl) << ctime(&end_time) << endl;

      if (prepro == "YES")
      {
         fsol << "--- PREPROCESSING ---" << endl << endl
              << std::fixed << std::setprecision(3)
              << WP("Elapsed time", wpl)
              << preproc->elapsedTime() << " (seconds)"
              << endl
              << std::fixed << std::setprecision(2)
              << WP("Status", wpl);

         cout << BLUE("Preprocessing") << endl;
         cout << std::fixed << std::setprecision(3)
              << indent << WP("Time", wpl)
              << GREEN(preproc->elapsedTime() << " (seconds)")
              << endl
              << std::fixed << std::setprecision(2)
              << indent << WP("Status", wpl);

         if (preproc->isSolved())
         {
            if (preproc->isUnfeasible())
            {
               fsol << "solved unfeasible" << endl;
               cout << RED("solved unfeasible") << endl;
            }
            else
            {
               fsol << "solved feasible" << endl;
               cout << GREEN("solved feasible") << endl;

               IntervalBox B(preproc->fixedRegion());
               fsol << endl << "SOLUTION " << std::scientific
                    << "[" << B.width() << "]"
                    << endl;
               B.listPrint(fsol);
            }
         }
         else
         {
            fsol << "checked" << endl;
            cout << GREEN("checked") << endl;

            fsol << WP("Number of variables fixed", wpl)
                 << preproc->nbFixedVars() << endl;

            cout << indent << WP("Number of variables fixed", wpl)
                 << GREEN(preproc->nbFixedVars()) << endl
                 << indent << WP("Number of inactive constraints", wpl)
                 << GREEN(preproc->nbInactiveCtrs()) << endl;

            if (preproc->nbFixedVars() > 0)
            {
               IntervalBox B(preproc->fixedRegion());
               Scope sco = preproc->fixedScope();
               B.listPrint(fsol);
            }

            fsol << WP("Number of inactive constraints", wpl)
                 << preproc->nbInactiveCtrs() << endl;

            if (preproc->nbInactiveCtrs() > 0)
            {
               for (size_t i=0; i<preproc->nbInactiveCtrs(); ++i)
               {
                  fsol << preproc->getInactiveCtr(i) << endl;
               }
            }
            fsol << endl;
         }
      }
      else
      {
         cout << BLUE("No preprocessing") << endl;
      }

      std::shared_ptr<NcspEnv> env = solver.getEnv();

      // solving
      if (!(prepro == "YES" && preproc->isSolved()))
      {
         NcspSpace* space = solver.getSpace();

         fsol << "--- SOLVING ---" << endl << endl;
         cout << GRAY(sep) << endl;
         cout << BLUE("Solving") << endl;

         fsol << WP("Elapsed time", wpl)
              << std::fixed << std::setprecision(3)
              << solver.getSolvingTime() << " (seconds)"
              << endl
              << WP("Number of nodes", wpl)
              << solver.getTotalNodes() << endl;

         cout << indent << WP("Time", wpl)
              << std::fixed << std::setprecision(3)
              << GREEN(solver.getSolvingTime() << " (seconds)")
              << endl
              << indent << WP("Number of nodes", wpl)
              << GREEN(solver.getTotalNodes()) << endl;

         bool complete = env->usedNoLimit() &&
                         space->nbPendingNodes() == 0;

         fsol << WP("Search status", wpl);
         cout << indent << WP("Search status", wpl);
         if (complete)
         {
            fsol << "complete" << endl;
            cout << GREEN("complete") << endl;
         }
         else
         {
            fsol << "partial" << endl;
            cout << RED("partial") << endl;
         }

         fsol << WP("Solution status", wpl);
         cout << indent << WP("Solution status", wpl);
         if (solver.nbSolutions() == 0)
         {
            if (complete)
            {
               fsol << "proved unfeasible" << endl;
               cout << RED("unfeasible") << endl;
            }
            else
            {
               fsol << "no solution found" << endl;
               cout << RED("no solution found") << endl;
            }
         }
         else
         {
            if (space->hasFeasibleSolNode())
            {
               fsol << "proved feasible" << endl;
               cout << GREEN("feasible") << endl;
            }
            else
            {
               fsol << "no proof certificate" << endl;
               cout << RED("no proof certificate") << endl;
            }
         }

         fsol << WP("Number of solutions", wpl)
              << solver.nbSolutions() << endl;

         if (solver.nbSolutions() > 0)
         {
            cout << indent << WP("Number of solutions", wpl)
                 << GREEN(solver.nbSolutions()) << endl;
         }
         else
         {
            cout << indent << WP("Number of solutions", wpl)
                 << RED(solver.nbSolutions()) << endl;
         }

         // limits
         if (env->usedTimeLimit())
         {
            double tl = env->getParam()->getDblParam("TIME_LIMIT");
            cout << std::fixed << std::setprecision(3);
            fsol << std::fixed << std::setprecision(3);
            cout << indent << WP("Time limit enabled", wpl)
                 << RED(tl << " (seconds)") << endl;
            fsol << WP("Time limit enabled", wpl) << tl << " (seconds)" << endl;
         }

         if (env->usedSolutionLimit())
         {
            int nb = env->getParam()->getIntParam("SOLUTION_LIMIT");
            cout << indent << WP("Solution limit enabled", wpl)
                 << RED(nb) << endl;
            fsol << WP("Solution limit enabled", wpl) << nb << endl;
         }

         if (env->usedNodeLimit())
         {
            int nb = env->getParam()->getIntParam("NODE_LIMIT");
            cout << indent << WP("Node limit enabled", wpl)
                 << RED(nb) << endl;
            fsol << WP("Node limit enabled", wpl) << nb << endl;
         }

         if (env->usedDepthLimit())
         {
            int nb = env->getParam()->getIntParam("DEPTH_LIMIT");
            cout << indent << WP("Depth limit enabled", wpl)
                 << RED(nb) << endl;
            fsol << WP("Depth limit enabled", wpl) << endl;
         }

         if (space->nbPendingNodes() > 0)
         {
            fsol << WP("Number of pending nodes", wpl)
                 << space->nbPendingNodes() << endl;
            cout << indent << WP("Number of pending nodes", wpl)
                 << RED(space->nbPendingNodes()) << endl;
         }

         // writes the solutions
         Scope sco = preproc->unfixedScope();

         int prec = prm.getIntParam("FLOAT_PRECISION");
         fsol << std::defaultfloat;

         string sd =solver.getEnv()->getParam()->getStrParam("DISPLAY_REGION");

         for (size_t i=0; i<solver.nbSolutions(); ++i)
         {
            std::pair<DomainBox, Proof> sol = solver.getSolution(i);
            IntervalBox hull(sol.first);

            fsol << std::setprecision(4);
            fsol << std::endl << "SOLUTION " << (i+1)
                 << " [" << hull.width() << "]";

            switch (sol.second)
            {
               case Proof::Inner:    fsol << " [exact]"; break;
               case Proof::Feasible: fsol << " [feasible]"; break;
               case Proof::Maybe:    fsol << " [unvalidated]"; break;
               default:              fsol << " (bug!!!)"; break;
            }

            fsol << std::setprecision(prec) << endl;
               
            if (sd == "STD")
               sol.first.listPrint(fsol);
            else
               sol.first.vecPrint(fsol);
         }

         // writes the hull of the pending nodes
         if (solver.nbPendingNodes() > 0)
         {
            IntervalBox hp(solver.getPendingBox(0));
            for (size_t i=1; i<solver.nbPendingNodes(); ++i)
            {
               IntervalBox aux(solver.getPendingBox(i));
               hp.glue(aux);
            }

            fsol << std::defaultfloat << std::setprecision(4);
            fsol << endl << "HULL OF PENDING NODES" << " ["
                 << hp.width() << "]" << endl;
            if (sd == "STD")
               hp.listPrint(fsol);

            else
               hp.vecPrint(fsol);
         }
      }

      // writes the problem
      fsol << endl << "--- INPUT PROBLEM ---" << endl << endl;
      DomainBox box(problem.scope());
      fsol << "BOX" << endl;
      box.listPrint(fsol);
      fsol << endl;

      // writes the constraints
      fsol << "CONSTRAINTS" << endl;
      for (size_t i=0; i<problem.nbCtrs(); ++i)
      {
         fsol << problem.ctrAt(i) << endl;
      }

      // writes the parameters in the solution file
      fsol << endl << "--- PARAMETERS ---" << endl << endl;
      env->getParam()->print(fsol);

      cout << GRAY(sep) << endl;

      // closes the solution file
      fsol.close();
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
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

void parseFilename(const std::string& filename, std::string& path,
                   std::string& base, std::string& ext)
{
   bool iter = true, found = false;
   size_t pos = 0;
   do
   {
      size_t k = filename.find('/', pos);
      if (k == std::string::npos)
      {
         k = filename.find('\\', pos);
      }
      if (k == std::string::npos)
      {
         iter = false;
      }
      else
      {
         pos = k+1;
         found = true;
      }
   }
   while (iter);

   if (found)
   {
      path = filename.substr(0, pos);
   }
   else
   {
      path = "";
   }
   
   size_t k = filename.find('.', pos);
   if (k == std::string::npos)
   {
      base = filename.substr(pos, filename.length() - pos);
      ext = "";
   }
   else
   {
      base = filename.substr(pos, k - pos);
      ext = filename.substr(k, filename.length() - k);
   }
}

string WP(const string& s, int n)
{
   string str = s;
   int m = n - s.length();
   while (--m >= 0) str += ".";
   str += " ";
   return str;
}
