#include "realpaver/BoxReporter.hpp"
#include "realpaver/Domain.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/Exporter.hpp"
#include "realpaver/ExporterHDF5.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Solver.hpp"
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

      LOG_MAIN("CSP solving");
      LOG_MAIN("Input file: " << pbFilename);
      if (paramFilename != "")
         LOG_MAIN("Parameter file: " << paramFilename);
      LOG_MAIN("Parsing...");

      // parsing
      Parser parser(params);
      Problem problem;

      try
      {
         ok = parser.parseFile(pbFilename, problem);
         if (!ok)
            THROW("Parse error: " << parser.getParseError());
      }
      catch (Exception &e)
      {
         THROW("Parse error: " << e.what());
      }

      LOG_MAIN("End of parsing");
      if (problem.nbCtrs() == 0)
         THROW("There is no constraint!");

      // bounds of the objective function can be given on the command line
      if (objBounds.isEmpty())
      {
         THROW("The bounds of the objective function are invalid");
      }

      if (problem.hasObjective())
      {
         Objective obj = problem.getObjective();
         Variable z = obj.getVar();
         Domain *dom = z.getDomain();
         dom->contract(objBounds);

         if (dom->isEmpty())
            THROW("The objective function has an empty domain");

         Constraint cobj(obj.getVar() == obj.getTerm());
         problem.addCtr(cobj);

         LOG_MAIN("Bounds of objective function: " << objBounds);
         LOG_MAIN("Constraint added: " << cobj);
      }
      else
      {
         if (objBounds.isSetNeq(Interval::universe()))
         {
            LOG_MAIN("Bound(s) given but there is no objective function")
         }
      }

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
      string prepro = solver.getEnv()->getParams()->getStrParam("PREPROCESSING");

      SharedPreprocessor preproc = solver.getPreprocessor();

      auto now = chrono::system_clock::now();
      std::time_t end_time = chrono::system_clock::to_time_t(now);

      if (prepro == "YES")
      {
         cout << BLUE("Preprocessing") << endl;
         cout << std::fixed << std::setprecision(3) << indent << WP("Time", wpl)
              << GREEN(preproc->elapsedTime() << " (seconds)") << endl
              << std::fixed << std::setprecision(2) << indent << WP("Status", wpl);

         if (preproc->isSolved())
         {
            if (preproc->isUnfeasible())
            {
               fsol << "solved infeasible" << endl;
               cout << RED("solved infeasible") << endl;
            }
            else
            {
               cout << GREEN("solved feasible") << endl;
            }
         }
         else
         {
            cout << GREEN("checked") << endl;

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
                  fsol << preproc->getInactiveCtr(i) << endl;
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
         std::shared_ptr<Space<Node>> space = solver.getSpace();

         cout << GRAY(sep) << endl;
         cout << BLUE("Solving") << endl;

         cout << indent << WP("Time", wpl) << std::fixed << std::setprecision(3)
              << GREEN(solver.getSolvingTime() << " (seconds)") << endl
              << indent << WP("Number of nodes", wpl) << GREEN(solver.getTotalNodes())
              << endl;

         bool complete = env->usedNoLimit() && space->nbPendingNodes() == 0;

         cout << indent << WP("Search status", wpl);
         if (complete)
         {
            cout << GREEN("complete") << endl;
         }
         else
         {
            cout << RED("partial") << endl;
         }

         cout << indent << WP("Solution status", wpl);
         if (solver.nbSolutions() == 0)
         {
            if (complete)
            {
               fsol << "proved infeasible" << endl;
               cout << RED("infeasible") << endl;
            }
            else
            {
               cout << RED("no solution found") << endl;
            }
         }
         else
         {
            if (space->hasFeasibleSolNode())
            {
               cout << GREEN("feasible") << endl;
            }
            else
            {
               cout << RED("no proof certificate") << endl;
            }
         }

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
            cout << indent << WP("Time limit enabled", wpl) << RED(tl << " (seconds)")
                 << endl;
         }

         if (env->usedSolutionLimit())
         {
            int nb = env->getParams()->getIntParam("SOLUTION_LIMIT");
            cout << indent << WP("Solution limit enabled", wpl) << RED(nb) << endl;
         }

         if (env->usedNodeLimit())
         {
            int nb = env->getParams()->getIntParam("NODE_LIMIT");
            cout << indent << WP("Node limit enabled", wpl) << RED(nb) << endl;
         }

         if (env->usedDepthLimit())
         {
            int nb = env->getParams()->getIntParam("DEPTH_LIMIT");
            cout << indent << WP("Depth limit enabled", wpl) << RED(nb) << endl;
         }

         if (space->nbPendingNodes() > 0)
            cout << indent << WP("Number of pending nodes", wpl)
                 << RED(space->nbPendingNodes()) << endl;
      }

      string output_format = solver.getEnv()->getParams()->getStrParam("OUTPUT_FORMAT");
      bool save_pendings =
          solver.getEnv()->getParams()->getStrParam("OUTPUT_PENDINGS") == "YES";
      if (output_format == "HDF5")
      {
         ExporterHDF5 h5(problem);
         h5.setSavePendings(save_pendings);
         if (solFilename == "")
            solFilename = baseFilename + ".h5";
         h5.save(solver, solFilename);
      }
      else // if (output_format == "SOL")
      {
         if (solFilename == "")
            solFilename = baseFilename + ".sol";
         ofstream fsol;
         fsol.open(solFilename, std::ofstream::out);
         if (fsol.bad())
            THROW("Open error of solution file");

         SharedPreprocessor preproc = solver.getPreprocessor();

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

            if (preproc->isSolved())
            {
               if (preproc->isUnfeasible())
               {
                  fsol << "solved unfeasible" << endl;
               }
               else
               {
                  fsol << "solved feasible" << endl;

                  IntervalBox B(preproc->fixedRegion());
                  fsol << endl
                       << "SOLUTION " << std::scientific << "[" << B.width() << "]"
                       << endl;
                  B.print(fsol);
               }
            }
            else
            {
               fsol << "checked" << endl;

               fsol << WP("Number of variables fixed", wpl) << preproc->nbFixedVars()
                    << endl;

               if (preproc->nbFixedVars() > 0)
               {
                  IntervalBox B(preproc->fixedRegion());
                  Scope sco = preproc->fixedScope();
                  B.print(fsol);
               }

               fsol << WP("Number of inactive constraints", wpl)
                    << preproc->nbInactiveCtrs() << endl;

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
         // solving
         if (!(prepro == "YES" && preproc->isSolved()))
         {
            SharedSpace space = solver.getSpace();

            bool complete = env->usedNoLimit() && space->nbPendingNodes() == 0;

            fsol << "--- SOLVING ---" << endl << endl;
            cout << GRAY(sep) << endl;
            cout << BLUE("Solving") << endl;

            fsol << WP("Elapsed time", wpl) << std::fixed << std::setprecision(3)
                 << solver.getSolvingTime() << " (seconds)" << endl
                 << WP("Number of nodes", wpl) << solver.getTotalNodes() << endl;

            fsol << WP("Search status", wpl);
            if (complete)
            {
               fsol << "complete" << endl;
            }
            else
            {
               fsol << "partial" << endl;
            }

            fsol << WP("Solution status", wpl);
            if (solver.nbSolutions() == 0)
            {
               if (complete)
               {
                  fsol << "proved unfeasible" << endl;
               }
               else
               {
                  fsol << "no solution found" << endl;
               }
            }
            else
            {
               if (space->hasFeasibleSolNode())
               {
                  fsol << "proved feasible" << endl;
               }
               else
               {
                  fsol << "no proof certificate" << endl;
               }
            }

            fsol << WP("Number of solutions", wpl) << solver.nbSolutions() << endl;

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
               fsol << WP("Node limit enabled", wpl) << nb << endl;
            }

            if (env->usedDepthLimit())
            {
               int nb = env->getParams()->getIntParam("DEPTH_LIMIT");
            }

            if (space->nbPendingNodes() > 0)

               fsol << WP("Number of pending nodes", wpl) << space->nbPendingNodes()
                    << endl;
            fsol << WP("Depth limit enabled", wpl) << endl;
         }

         // writes the solutions
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
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
