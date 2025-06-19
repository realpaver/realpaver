#include "realpaver/BoxReporter.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Solver.hpp"
#include "realpaver/configure.hpp"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace realpaver;
using namespace std;

#define BLACK(s) "\033[30m" << s << "\033[39m"
#define RED(s) "\033[31m" << s << "\033[39m"
#define GREEN(s) "\033[32m" << s << "\033[39m"
#define ORANGE(s) "\033[33m" << s << "\033[39m"
#define BLUE(s) "\033[34m" << s << "\033[39m"
#define MAGENTA(s) "\033[35m" << s << "\033[39m"
#define CYAN(s) "\033[36m" << s << "\033[39m"
#define GRAY(s) "\033[37m" << s << "\033[39m"

// processes the arguments on the command line
bool processArgs(int argc, char **argv, string &pbFilename, string &solFilename,
                 string &paramFilename, string &logFilename, vector<string> &options,
                 string &msgError);

// parses the filename
void parseFilename(const std::string &filename, std::string &path, std::string &base,
                   std::string &ext);

// processes the options to assign parameters
bool processOptions(Params &params, vector<string> &options, string &msgError);

// replaces a tilde by the home directory in a filename.
bool expandTildeInPath(const string &path, string &res, string &msgError);

// inserts points at the end of a string
string WP(const string &s, int n);

// main function
int main(int argc, char **argv)
{
   try
   {
      string pbFilename = "", paramFilename = "", solFilename = "", logFilename = "",
             msgError = "";
      vector<string> options;

      // processes the arguments
      bool ok = processArgs(argc, argv, pbFilename, solFilename, paramFilename,
                            logFilename, options, msgError);
      if (!ok)
         THROW(msgError);

      // parses the problem file name
      string baseFilename, pathFilename, extFilename;
      parseFilename(pbFilename, pathFilename, baseFilename, extFilename);

      // tries to open the problem file
      ifstream infile(pbFilename);
      if (!infile.is_open())
         THROW("Bad problem filename: " << pbFilename);
      else
         infile.close();

      // reads the parameters
      Params params;
      if (paramFilename != "")
         params.loadParam(paramFilename);
      ok = processOptions(params, options, msgError);
      if (!ok)
         THROW(msgError);

      // logger
#if LOG_ON
      LogLevel loglevel = StringToLogLevel(params.getStrParam("LOG_LEVEL"));
      if (loglevel != LogLevel::none)
      {
         cout << "logFilename : (" << logFilename << ")" << endl;

         if (logFilename == "")
            logFilename = baseFilename + ".log";
         Logger::init(loglevel, logFilename);
      }
#endif

      LOG_MAIN("CSP solving");
      LOG_MAIN("Input file: " << pbFilename);
      if (paramFilename != "")
         LOG_MAIN("Parameter file: " << paramFilename);
      LOG_MAIN("Parsing...");

      // parsing
      Parser parser(params);
      Problem problem;

      ok = parser.parseFile(pbFilename, problem);
      if (!ok)
         THROW("Parse error: " << parser.getParseError());

      LOG_MAIN("End of parsing");
      if (problem.nbCtrs() == 0)
         THROW("There is no constraint!");

      if (problem.hasObjective())
         LOG_MAIN("The objective function is not considered by the solver.");

      // solving
      Solver solver(problem);
      solver.getEnv()->setParams(params);

      std::string sep = "########################################";
      sep += sep;
      std::string indent = "   ";
      int wpl = 36;

      cout << GRAY(sep) << endl;
      cout << BLUE(REALPAVER_STRING) << BLUE(" CSP solver") << endl;

      ////////////////////
      solver.solve();
      ////////////////////

      if (solFilename == "")
         solFilename = baseFilename + ".sol";
      ofstream fsol;
      fsol.open(solFilename, std::ofstream::out);
      if (fsol.bad())
         THROW("Open error of solution file");

      // preliminaries
      cout << GRAY(sep) << endl;
      cout << BLUE("Files") << endl;

      cout << indent << WP("Input file", wpl) << BLACK(pbFilename) << endl;

      if (paramFilename != "")
         cout << indent << WP("Parameter file", wpl) << BLACK(paramFilename) << endl;

#if LOG_ON
      cout << indent << WP("Log file", wpl);
      string meslog = (loglevel != LogLevel::none) ? logFilename : "no log";
      cout << BLACK(meslog) << endl;
#endif

      cout << indent << WP("Output file", wpl) << BLACK(solFilename) << endl;

      // preprocessing
      cout << GRAY(sep) << endl;
      string prepro = solver.getEnv()->getParams()->getStrParam("PREPROCESSING");

      Preprocessor *preproc = solver.getPreprocessor();

      fsol << WP("CSP solver", wpl) << REALPAVER_STRING << endl;

      auto now = chrono::system_clock::now();
      std::time_t end_time = chrono::system_clock::to_time_t(now);

      fsol << WP("Input file", wpl) << pbFilename << endl;
      fsol << WP("Current date and time", wpl) << ctime(&end_time) << endl;

      if (prepro == "YES")
      {
         fsol << "--- PREPROCESSING ---" << endl
              << endl
              << std::fixed << std::setprecision(3) << WP("Elapsed time", wpl)
              << preproc->elapsedTime() << " (seconds)" << endl
              << std::fixed << std::setprecision(2) << WP("Status", wpl);

         cout << BLUE("Preprocessing") << endl;
         cout << std::fixed << std::setprecision(3) << indent << WP("Time", wpl)
              << GREEN(preproc->elapsedTime() << " (seconds)") << endl
              << std::fixed << std::setprecision(2) << indent << WP("Status", wpl);

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
               fsol << endl
                    << "SOLUTION " << std::scientific << "[" << B.width() << "]" << endl;
               B.print(fsol);
            }
         }
         else
         {
            fsol << "checked" << endl;
            cout << GREEN("checked") << endl;

            fsol << WP("Number of variables fixed", wpl) << preproc->nbFixedVars()
                 << endl;

            cout << indent << WP("Number of variables fixed", wpl)
                 << GREEN(preproc->nbFixedVars()) << endl
                 << indent << WP("Number of inactive constraints", wpl)
                 << GREEN(preproc->nbInactiveCtrs()) << endl;

            if (preproc->nbFixedVars() > 0)
            {
               IntervalBox B(preproc->fixedRegion());
               Scope sco = preproc->fixedScope();
               B.print(fsol);
            }

            fsol << WP("Number of inactive constraints", wpl) << preproc->nbInactiveCtrs()
                 << endl;

            if (preproc->nbInactiveCtrs() > 0)
            {
               for (size_t i = 0; i < preproc->nbInactiveCtrs(); ++i)
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

      std::shared_ptr<EnvSolver> env = solver.getEnv();

      // solving
      if (!(prepro == "YES" && preproc->isSolved()))
      {
         Space *space = solver.getSpace();

         fsol << "--- SOLVING ---" << endl << endl;
         cout << GRAY(sep) << endl;
         cout << BLUE("Solving") << endl;

         fsol << WP("Elapsed time", wpl) << std::fixed << std::setprecision(3)
              << solver.getSolvingTime() << " (seconds)" << endl
              << WP("Number of nodes", wpl) << solver.getTotalNodes() << endl;

         cout << indent << WP("Time", wpl) << std::fixed << std::setprecision(3)
              << GREEN(solver.getSolvingTime() << " (seconds)") << endl
              << indent << WP("Number of nodes", wpl) << GREEN(solver.getTotalNodes())
              << endl;

         bool complete = env->usedNoLimit() && space->nbPendingNodes() == 0;

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

         fsol << WP("Number of solutions", wpl) << solver.nbSolutions() << endl;

         if (solver.nbSolutions() > 0)
         {
            cout << indent << WP("Number of solutions", wpl)
                 << GREEN(solver.nbSolutions()) << endl;
         }
         else
         {
            cout << indent << WP("Number of solutions", wpl) << RED(solver.nbSolutions())
                 << endl;
         }

         // limits
         if (env->usedTimeLimit())
         {
            double tl = env->getParams()->getDblParam("TIME_LIMIT");
            cout << std::fixed << std::setprecision(3);
            fsol << std::fixed << std::setprecision(3);
            cout << indent << WP("Time limit enabled", wpl) << RED(tl << " (seconds)")
                 << endl;
            fsol << WP("Time limit enabled", wpl) << tl << " (seconds)" << endl;
         }

         if (env->usedSolutionLimit())
         {
            int nb = env->getParams()->getIntParam("SOLUTION_LIMIT");
            cout << indent << WP("Solution limit enabled", wpl) << RED(nb) << endl;
            fsol << WP("Solution limit enabled", wpl) << nb << endl;
         }

         if (env->usedNodeLimit())
         {
            int nb = env->getParams()->getIntParam("NODE_LIMIT");
            cout << indent << WP("Node limit enabled", wpl) << RED(nb) << endl;
            fsol << WP("Node limit enabled", wpl) << nb << endl;
         }

         if (env->usedDepthLimit())
         {
            int nb = env->getParams()->getIntParam("DEPTH_LIMIT");
            cout << indent << WP("Depth limit enabled", wpl) << RED(nb) << endl;
            fsol << WP("Depth limit enabled", wpl) << endl;
         }

         if (space->nbPendingNodes() > 0)
         {
            fsol << WP("Number of pending nodes", wpl) << space->nbPendingNodes() << endl;
            cout << indent << WP("Number of pending nodes", wpl)
                 << RED(space->nbPendingNodes()) << endl;
         }

         // writes the solutions
         Scope sco = preproc->unfixedScope();

         std::string dr = params.getStrParam("DISPLAY_REGION");

         StreamReporter reporter(problem, fsol);
         for (const auto &v : sco)
            if (!problem.isVarReported(v))
               reporter.remove(v.getName());

         if (dr == "STD")
            reporter.setVertical(true);
         if (dr == "VEC")
            reporter.setVertical(false);

         std::string sd = params.getStrParam("DISPLAY_REGION");

         int prec = params.getIntParam("FLOAT_PRECISION");
         fsol << std::defaultfloat;

         for (size_t i = 0; i < solver.nbSolutions(); ++i)
         {
            if (i > 0)
               fsol << std::endl;

            std::pair<DomainBox, Proof> sol = solver.getSolution(i);
            IntervalBox hull(sol.first);

            fsol << std::setprecision(4);
            fsol << std::endl << "SOLUTION " << (i + 1) << " [" << hull.width() << "]";

            switch (sol.second)
            {
            case Proof::Inner:
               fsol << " [exact]";
               break;
            case Proof::Feasible:
               fsol << " [feasible]";
               break;
            case Proof::Maybe:
               fsol << " [unvalidated]";
               break;
            default:
               fsol << " (bug!!!)";
               break;
            }

            fsol << std::setprecision(prec) << endl;
            reporter.report(sol.first);
         }

         // writes the hull of the pending nodes
         if (solver.nbPendingNodes() > 0)
         {
            IntervalBox hp(solver.getPendingBox(0));
            for (size_t i = 1; i < solver.nbPendingNodes(); ++i)
            {
               IntervalBox aux(solver.getPendingBox(i));
               hp.glue(aux);
            }

            fsol << std::defaultfloat << std::setprecision(4);
            fsol << endl << "HULL OF PENDING NODES" << " [" << hp.width() << "]" << endl;
            hp.print(fsol);
         }
      }

      // writes the problem
      fsol << endl << "--- INPUT PROBLEM ---" << endl << endl;
      DomainBox box(problem.scope());
      fsol << "BOX" << endl;
      box.print(fsol);
      fsol << endl;

      // writes the constraints
      fsol << "CONSTRAINTS" << endl;
      for (size_t i = 0; i < problem.nbCtrs(); ++i)
      {
         fsol << problem.ctrAt(i) << endl;
      }

      // writes the parameters in the solution file
      fsol << endl << "--- PARAMETERS ---" << endl << endl;
      env->getParams()->printValues(fsol);

      cout << GRAY(sep) << endl;

      // closes the solution file
      fsol.close();
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}

bool processArgs(int argc, char **argv, string &pbFilename, string &solFilename,
                 string &paramFilename, string &logFilename, vector<string> &options,
                 string &msgError)
{
   int i = 1;
   bool ok = true;

   pbFilename = "";
   paramFilename = "";
   solFilename = "";
   logFilename = "";
   options.clear();

   while (i < argc && ok)
   {
      std::string text(argv[i]);

      if (text.length() > 2 && text.substr(0, 2) == "-P")
      {
         if (paramFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            string str = text.substr(2, text.length() - 2);
            ok = expandTildeInPath(str, paramFilename, msgError);

            if (ok)
            {
               ifstream f(paramFilename);
               if (f)
                  f.close();

               else
               {
                  msgError = "Bad argument: " + text;
                  ok = false;
               }
            }
         }
      }
      else if (text.length() > 2 && text.substr(0, 2) == "-L")
      {
         if (logFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            string str = text.substr(2, text.length() - 2);
            ok = expandTildeInPath(str, logFilename, msgError);
         }
      }
      else if (text.length() > 2 && text.substr(0, 2) == "-O")
      {
         if (solFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            string str = text.substr(2, text.length() - 2);
            ok = expandTildeInPath(str, solFilename, msgError);
         }
      }
      else if (text.length() > 2 && text.substr(0, 2) == "-D")
      {
         options.push_back(text.substr(2, text.length() - 2));
      }
      else
      {
         if (pbFilename != "")
         {
            msgError = "Bad argument: " + text;
            ok = false;
         }
         else
         {
            pbFilename = text;
            ifstream f(pbFilename);
            if (f)
            {
               f.close();
            }
            else
            {
               msgError = "Bad argument: " + text;
               ok = false;
            }
         }
      }
      i++;
   }

   if (pbFilename == "")
   {
      msgError = "No problem file!";
      ok = false;
   }

   return ok;
}

void parseFilename(const std::string &filename, std::string &path, std::string &base,
                   std::string &ext)
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
         pos = k + 1;
         found = true;
      }
   } while (iter);

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

bool processOptions(Params &params, vector<string> &options, string &msgError)
{
   for (const string &opt : options)
   {
      if (!params.readParam(opt))
      {
         msgError = "Bad option: " + opt;
         return false;
      }
   }
   return true;
}

bool expandTildeInPath(const string &path, string &res, string &msgError)
{
   res = "";
   for (char c : path)
   {
      if (c == '~')
      {
         const char *home = std::getenv("HOME");
         if (home)
         {
            res += string(home);
         }
         else
         {
            msgError = "Not able to replace ~ by HOME in " + path;
            return false;
         }
      }
      else
      {
         res += string(1, c);
      }
   }
   return true;
}

string WP(const string &s, int n)
{
   string str = s;
   int m = n - s.length();
   while (--m >= 0)
      str += ".";
   str += " ";
   return str;
}
