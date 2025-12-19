#include "Common.hpp"
#include "Domain.hpp"
#include "realpaver/BoxReporter.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Optimizer.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/configure.hpp"
#include "rp_util.hpp"
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace realpaver;
using namespace std;

// main function
int main(int argc, char **argv)
{
   try
   {
      string pbFilename = "", paramFilename = "", solFilename = "", logFilename = "",
             msgError = "";
      vector<string> options;
      Interval objBounds = Interval::universe();

      // processes the arguments
      bool ok = processArgs(argc, argv, pbFilename, solFilename, paramFilename,
                            logFilename, options, objBounds, msgError);
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
#if REALPAVER_LOG_ON
      LogLevel loglevel = StringToLogLevel(params.getStrParam("LOG_LEVEL"));
      if (loglevel != LogLevel::none)
      {
         if (logFilename == "")
            logFilename = baseFilename + ".log";
         Logger::init(loglevel, logFilename);
      }
#endif

      LOG_MAIN("Constrained optimization");
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

      if (!problem.hasObjective())
      {
         THROW("There is no objective function!");
      }

      // bounds of the objective function can be given on the command line
      if (objBounds.isEmpty())
      {
         THROW("The bounds of the objective function are invalid");
      }
      else
      {
         Objective obj = problem.getObjective();
         Variable z = obj.getVar();
         Domain *dom = z.getDomain();
         dom->contract(objBounds);

         if (dom->isEmpty())
            THROW("The objective function has an empty domain");

         LOG_MAIN("Bounds of objective function: " << objBounds);
      }

      // solving
      Optimizer bbopt(problem);
      bbopt.getEnv()->setParams(params);

      std::string sep = "########################################";
      sep += sep;
      std::string indent = "   ";
      int wpl = 36;

      cout << GRAY(sep) << endl;
      cout << BLUE(REALPAVER_STRING) << BLUE(" COP solver") << endl;

      ////////////////////
      bbopt.optimize();
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

#if REALPAVER_LOG_ON
      cout << indent << WP("Log file", wpl);
      string meslog = (loglevel != LogLevel::none) ? logFilename : "no log";
      cout << BLACK(meslog) << endl;
#endif

      cout << indent << WP("Output file", wpl) << BLACK(solFilename) << endl;

      // preprocessing
      cout << GRAY(sep) << endl;
      string prepro = bbopt.getEnv()->getParams()->getStrParam("PREPROCESSING");

      SharedPreprocessor preproc = bbopt.getPreprocessor();

      fsol << WP("COP solver", wpl) << REALPAVER_STRING << endl;

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
               fsol << std::endl;
            }

            fsol << WP("Number of inactive constraints", wpl) << preproc->nbInactiveCtrs()
                 << endl;

            if (preproc->nbInactiveCtrs() > 0)
            {
               for (size_t i = 0; i < preproc->nbInactiveCtrs(); ++i)
                  fsol << preproc->getInactiveCtr(i) << endl;
            }
            fsol << endl;
         }
      }
      else
      {
         cout << BLUE("No preprocessing") << endl;
      }

      std::shared_ptr<EnvSolver> env = bbopt.getEnv();

      // solving
      if (!(prepro == "YES" && preproc->isSolved()))
      {
         std::shared_ptr<Space<NodeOptim>> space = bbopt.getSpace();

         fsol << "--- SOLVING ---" << endl << endl;
         cout << GRAY(sep) << endl;
         cout << BLUE("Solving") << endl;

         fsol << WP("Elapsed time", wpl) << std::fixed << std::setprecision(3)
              << bbopt.getSolvingTime() << " (seconds)" << endl
              << WP("Number of nodes", wpl) << bbopt.getTotalNodes() << endl;

         cout << indent << WP("Time", wpl) << std::fixed << std::setprecision(3)
              << GREEN(bbopt.getSolvingTime() << " (seconds)") << endl
              << indent << WP("Number of nodes", wpl) << GREEN(bbopt.getTotalNodes())
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

         fsol << WP("Optimization status", wpl);
         cout << indent << WP("Optimization status", wpl);

         OptimizationStatus status = bbopt.getStatus();
         fsol << status << endl;

         if (status == OptimizationStatus::Optimal)
            cout << GREEN(status) << endl;
         else
            cout << RED(status) << endl;

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

         // writes the solution
         Scope sco = preproc->isOn() ? preproc->unfixedScope() : problem.scope();
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

         if (status == OptimizationStatus::Optimal)
         {
            fsol << "\nOptimum enclosure: " << std::setprecision(prec)
                 << bbopt.getOptimum() << endl;
            fsol << "\nBest feasible point: " << std::setprecision(prec) << endl;
            IntervalBox B(bbopt.bestFeasible());
            reporter.report(B);
         }
      }

      // writes the problem
      fsol << endl << "--- INPUT PROBLEM ---" << endl << endl;
      DomainBox box(problem.scope());
      fsol << "BOX" << endl;
      box.print(fsol);
      fsol << endl;

      // writes the objective
      fsol << "OBJECTIVE FUNCTION" << endl;
      fsol << problem.getObjective() << endl;

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
