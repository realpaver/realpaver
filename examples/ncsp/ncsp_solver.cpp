#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "realpaver/config.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Stat.hpp"

using namespace realpaver;
using namespace std;

#define BLACK(s) "\033[30m" << s << "\033[39m"
#define RED(s)   "\033[31m" << s << "\033[39m"
#define BLUE(s)  "\033[34m" << s << "\033[39m"
#define GRAY(s)  "\033[37m" << s << "\033[39m"
#define GREEN(s) "\033[32m" << s << "\033[39m"
#define ORANGE(s) "\033[33m" << s << "\033[39m"

// returns the filename without its extension
string baseFilename(const std::string& filename);

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
      string basefname = baseFilename(filename);

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
         flog = basefname + ".log";
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

      int prec = prm.getIntParam("FLOAT_PRECISION");
      Interval::precision(prec);

      ////////////////////
      solver.solve();
      ////////////////////

      string solfilename = basefname + ".sol";
      ofstream fsol;
      fsol.open(solfilename, std::ofstream::out);
      if (fsol.bad()) THROW("Open error of solution file");

      std::string sep = "###################################";
      sep += sep;
      std::string indent = "   ";
      int wpl = 36;

      // preliminaries
      cout << GRAY(sep) << endl;
      cout << BLUE("Realpaver NCSP solving") << endl;

      cout << indent << WP("Input file", wpl) << ORANGE(filename) << endl;

#if LOG_ON
      cout << indent << WP("Log file", wpl);
      string meslog = (loglevel != LogLevel::none) ? flog : "no log";
      cout << ORANGE(meslog) << endl;
#endif

      cout << indent << WP("Output file", wpl) << ORANGE(solfilename) << endl;
      cout << GRAY(sep) << endl;

      // preprocessing
      string prepro = solver.getEnv()->getParam()->getStrParam("PREPROCESSING");

      Preprocessor* preproc = solver.getPreprocessor();

      fsol << WP("NCSP solver", wpl) << REALPAVER_STRING << endl;

      auto now = chrono::system_clock::now();
      std::time_t end_time = chrono::system_clock::to_time_t(now);

      fsol << WP("Input file", wpl) << filename << endl;
      fsol << WP("Current date and time: ", wpl) << ctime(&end_time) << endl;

      if (prepro == "YES")
      {
         fsol << "--- PREPROCESSING ---" << endl << endl
              << std::fixed << std::setprecision(2)
              << WP("Elapsed time", wpl)
              << preproc->elapsedTime() << " (seconds)"
              << endl
              << std::fixed << std::setprecision(2)
              << WP("Status", wpl);

         cout << BLUE("Preprocessing") << endl;
         cout << std::fixed << std::setprecision(2)
              << indent << WP("Time", wpl)
              << ORANGE(preproc->elapsedTime() << "s")
              << endl
              << std::fixed << std::setprecision(2)
              << indent << WP("Status", wpl);

         if (preproc->isSolved())
         {
            if (preproc->isUnfeasible())
            {
               fsol << "solved unfeasible" << endl;
               cout << ORANGE("solved unfeasible") << endl;
            }
            else
            {
               fsol << "solved feasible" << endl;
               cout << ORANGE("solved feasible") << endl;
            }
         }
         else
         {
            fsol << "not solved" << endl;
            cout << ORANGE("not solved") << endl;
         }

         if (!preproc->isSolved())
         {
            fsol << WP("Number of inactive constraints", wpl)
                 << preproc->nbInactiveCtrs() << endl
                 << WP("Number of variables fixed", wpl)
                 << preproc->nbFixedVars() << endl;
            
            cout << indent << WP("Number of variables fixed", wpl)
                 << ORANGE(preproc->nbFixedVars()) << endl
                 << indent << WP("Number of inactive constraints", wpl)
                 << ORANGE(preproc->nbInactiveCtrs()) << endl;
         }

         cout << GRAY(sep) << endl;

         if (preproc->nbFixedVars() > 0)
         {
            fsol << endl;
            IntervalRegion reg(preproc->fixedRegion());
            Scope sco = preproc->fixedScope();
            reg.stdPrint(fsol);
            fsol << endl;
         }
      }

      // solving
      if (!(prepro == "YES" && preproc->isSolved()))
      {
         NcspEnv* env = solver.getEnv();
         NcspSpace* space = solver.getSpace();

         fsol << "--- SOLVING ---" << endl << endl;
         cout << BLUE("Solving") << endl;

         fsol << WP("Elapsed time", wpl)
              << std::fixed << std::setprecision(2)
              << solver.getSolvingTime() << " (seconds)"
              << endl
              << WP("Number of nodes", wpl)
              << solver.getTotalNodes() << endl;

         cout << indent << WP("Time", wpl)
              << std::fixed << std::setprecision(2)
              << ORANGE(solver.getSolvingTime() << "s")
              << endl
              << indent << WP("Number of nodes", wpl)
              << ORANGE(solver.getTotalNodes()) << endl;

         bool complete = env->usedNoLimit() &&
                         space->nbPendingNodes() == 0;

         fsol << WP("Search status", wpl);
         cout << indent << WP("Search status", wpl);
         if (complete)
         {
            fsol << "complete" << endl;
            cout << ORANGE("complete") << endl;
         }
         else
         {
            fsol << "partial" << endl;
            cout << ORANGE("partial") << endl;
         }

         fsol << WP("Solution status", wpl);
         cout << indent << WP("Solution status", wpl);
         if (space->nbSolNodes() == 0)
         {
            if (complete)
            {
               fsol << "proved unfeasible" << endl;
               cout << ORANGE("unfeasible") << endl;
            }
            else
            {
               fsol << "no solution found" << endl;
               cout << ORANGE("no solution found") << endl;
            }
         }
         else
         {
            if (space->hasFeasibleSolNode())
            {
               fsol << "proved feasible" << endl;
               cout << ORANGE("feasible") << endl;
            }
            else
            {
               fsol << "no proof certificate" << endl;
               cout << ORANGE("no proof certificate") << endl;
            }

            fsol << WP("Number of clustered solutions", wpl)
                 << space->nbSolNodes() << endl;
            cout << indent << WP("Number of clustered solutions", wpl)
                 << ORANGE(space->nbSolNodes()) << endl;

            fsol << WP("Number of individual solutions", wpl)
                 << space->nbTotalSolNodes() << endl;
            cout << indent << WP("Number of individual solutions", wpl)
                 << ORANGE(space->nbTotalSolNodes()) << endl;

            if (!complete)
            {
               fsol << WP("Number of pending nodes", wpl)
                    << space->nbPendingNodes() << endl;
               cout << indent << WP("Number of pending nodes", wpl)
                    << ORANGE(space->nbPendingNodes()) << endl;
            }

            // writes the solutions
            Scope sco = preproc->unfixedScope();

            string sdis =
               solver.getEnv()->getParam()->getStrParam("DISPLAY_REGION");
            if (sdis == "STD")
            {
               for (size_t i=0; i<space->nbSolNodes(); ++i)
               {
                  SharedNcspNode node = space->getSolNode(i);
                  IntervalRegion* reg = node->region();
                  Proof proof = node->getProof();

                  fsol << std::endl << "SOLUTION " << (i+1)
                       << " [" << reg->width() << "]";

                  switch (proof)
                  {
                     case Proof::Inner:    fsol << " [inner]"; break;
                     case Proof::Feasible: fsol << " [safe]"; break;
                     case Proof::Maybe:    fsol << " [unsafe]"; break;
                     default:              fsol << " (bug!!!)"; break;
                  }

                  fsol << endl;
                  reg->stdPrint(fsol);                  
               }
            }
            else if (sdis == "VEC")
            {
               fsol << endl << "SCOPE = " << sco << endl;
               for (size_t i=0; i<space->nbSolNodes(); ++i)
               {
                  SharedNcspNode node = space->getSolNode(i);
                  IntervalRegion* reg = node->region();
                  fsol << endl;
                  reg->vecPrint(fsol);
               }
            }
            else
            {
               THROW("Bad parameter value: DISPLAY_REGION = " << sdis);
            }

            // writes the hull of the pending nodes
            if (space->nbPendingNodes() > 0)
            {
               fsol << endl << "HULL OF PENDING NODES" << endl;
               if (sdis == "STD")
                  space->hullOfPendingNodes().stdPrint(fsol);
               else if (sdis == "VEC")
                  space->hullOfPendingNodes().vecPrint(fsol);
            }
         }

         // writes the parameters in the solution file
         fsol << endl << "--- PARAMETERS ---" << endl << endl;
         env->getParam()->print(fsol);

         // limits
         if (env->usedTimeLimit())
            cout << indent << WP("Time limit enabled", wpl)
                 << ORANGE(env->getParam()->getDblParam("TIME_LIMIT")) << endl;
         
         if (env->usedSolutionLimit())
            cout << indent << WP("Solution limit enabled", wpl)
                 << ORANGE(env->getParam()->getIntParam("SOLUTION_LIMIT"))
                 << endl;

         if (env->usedNodeLimit())
            cout << indent << WP("Node limit enabled", wpl)
                 << ORANGE(env->getParam()->getIntParam("NODE_LIMIT")) << endl;

         if (env->usedDepthLimit())
            cout << indent << WP("Depth limit enabled", wpl)
                 << ORANGE(env->getParam()->getIntParam("DEPTH_LIMIT")) << endl;

         cout << GRAY(sep) << endl;
      }

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

string baseFilename(const std::string& filename)
{
   string str = "";
   if (filename[0] == '.') return str;

   bool hasPoint = false;

   // finds an extension if any
   int i = filename.length() - 1;
   while (i>=0 and !hasPoint)
   {
      if (filename[i] == '.')
      {
         hasPoint = true;
         str = filename.substr(0, i);
      }
      i = i-1;
   }

   if (!hasPoint) str = filename;

   return str;
}

string WP(const string& s, int n)
{
   string str = s;
   int m = n - s.length();
   while (--m >= 0) str += ".";
   str += " ";
   return str;
}
