#include <fstream>
#include <iomanip>
#include <iostream>
#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/NcspSpaceWriter.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Parser.hpp"

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
      Problem problem;
      Parser parser;
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

      // assigns the default tolerance of the real variables (before parsing)
      Param::SetTolParam("XTOL", prm.getTolParam("XTOL"));

      // logger
      LogLevel loglevel = StringToLogLevel(prm.getStrParam("LOG_LEVEL"));
      string flog = "";
      if (loglevel != LogLevel::none)
      {
         flog = basefname + ".log";
         Logger::init(loglevel, flog);
      }

      LOG_MAIN("NCSP solving");
      LOG_MAIN("Input file: " << filename);

      // parsing
      ok = parser.parseFile(filename, problem);
      if (!ok) THROW("Parse error: " << parser.getParseError());
      if (!problem.isCSP()) THROW("Not a NCSP");

      // solving
      NcspSolver solver(problem);
      solver.getEnv()->setParam(prm);

      int prec = prm.getIntParam("FLOAT_PRECISION");
      Interval::precision(prec);

      solver.solve();      

      string fsol = basefname + ".sol";

      std::string sep = "##############################";
      sep += sep;
      std::string indent = "   ";
      int wpl = 36;

      // preliminaries
      cout << GRAY(sep) << endl;
      cout << BLUE("Realpaver NCSP solving") << endl;

      cout << indent << WP("Input file", wpl) << ORANGE(filename) << endl;

      cout << indent << WP("Log file", wpl);
      string meslog = (loglevel != LogLevel::none) ? flog : "no log";
      cout << ORANGE(meslog) << endl;

      cout << indent << WP("Output file", wpl) << ORANGE(fsol) << endl;
      cout << GRAY(sep) << endl;

      // preprocessing
      Preprocessor* preproc = solver.getPreprocessor();

      cout << BLUE("Preprocessing") << endl;
      cout << std::fixed << std::setprecision(2)
           << indent << WP("Time", wpl)
           << ORANGE(preproc->elapsedTime() << "s")
           << endl;

      cout << std::fixed << std::setprecision(2)
           << indent << WP("Status", wpl);

      if (preproc->isSolved())
      {
         if (preproc->isUnfeasible())
            cout << ORANGE("solved unfeasible") << endl;
         else
            cout << ORANGE("solved feasible") << endl;
      }
      else
         cout << ORANGE("not solved") << endl;

      if (!preproc->isSolved())
      {
         cout << indent << WP("Number of variables fixed", wpl)
              << ORANGE(preproc->nbFixedVars()) << endl
              << indent << WP("Number of inactive constraints", wpl)
              << ORANGE(preproc->nbInactiveCtrs()) << endl;
      }

      cout << GRAY(sep) << endl;

      if (preproc->nbFixedVars() > 0)
      {
         IntervalRegion reg(preproc->fixedRegion());
         
         // TODO: writes it
      }


      // solving
      if (!preproc->isSolved())
      {
         cout << BLUE("Solving") << endl;

         cout << indent << WP("Time", wpl)
              << std::fixed << std::setprecision(2)
              << ORANGE(solver.getSolvingTime() << "s")
              << endl
              << indent << WP("Number of nodes", wpl)
              << ORANGE(solver.getTotalNodes()) << endl;

      }


      //~ // solving effort
      //~ cout << GRAY(sep) << endl;
      //~ cout << BLUE("Solving effort") << endl;
      //~ cout << std::fixed << std::setprecision(2)
           //~ << indent << "Preprocessing time.......... "
           //~ << ORANGE(solver.getPreprocessingTime() << "s")
           //~ << endl;

      //~ cout << indent << "Solving time................ "
           //~ << std::fixed << std::setprecision(2)
           //~ << ORANGE(solver.getSolvingTime() << "s")
           //~ << endl
           //~ << indent << "Number of nodes............. "
           //~ << ORANGE(solver.getTotalNodes()) << endl;

      //~ // results
      //~ cout << GRAY(sep) << endl;
      //~ cout << BLUE("Results") << endl;

//~ cerr << "LA" << solver.getSpace()->nbPendingNodes() << endl;

//~ // TODO : plante car le space n'a pas été créé car probleme
//~ // resolu au preprocessing

      //~ cout << indent << "Search status............... ";
      //~ bool complete = solver.getEnv()->usedNoLimit() &&
                      //~ (solver.getSpace()->nbPendingNodes() == 0);


//~ cerr << "ICI" << endl;

      //~ if (complete)
         //~ cout << ORANGE("complete") << endl;
      //~ else
         //~ cout << ORANGE("partial") << endl;
   
      //~ cout << indent << "Solution status............. ";

      //~ if (solver.getSpace()->nbSolutionNodes() == 0)
      //~ {
         //~ string s = complete ? "infeasible" : "no solution found";
         //~ cout << ORANGE(s) << endl;
      //~ }
      //~ else
      //~ {
         //~ string s = solver.getSpace()->hasFeasibleSolutionNode() ? "feasible" :
                                                       //~ "no proof certificate";
         //~ cout << ORANGE(s) << endl;

         //~ cout << indent << "Number of solutions......... "
              //~ << ORANGE(solver.getSpace()->nbSolutionNodes()) << endl;
      //~ }

      //~ // Writes the space / solutions in a file
      //~ NcspSpaceFileWriter writer(fsol);
      //~ writer.write(*solver.getSpace());
      
      //~ cout << indent << "Number of pending nodes..... "
           //~ << ORANGE(solver.getSpace()->nbPendingNodes()) << endl;

      //~ // limits
      //~ if (solver.getEnv()->usedTimeLimit())
      //~ {
         //~ cout << indent << "Time limit reached.......... "
              //~ << solver.getEnv()->getParam()->getDblParam("TIME_LIMIT") << endl;
      //~ }
      //~ if (solver.getEnv()->usedNodeLimit())
      //~ {
         //~ cout << indent << "Node limit reached.......... "
              //~ << solver.getEnv()->getParam()->getIntParam("NODE_LIMIT") << endl;
      //~ }
      //~ if (solver.getEnv()->usedNodeLimit())
      //~ {
         //~ cout << indent << "Solution limit reached...... "
              //~ << solver.getEnv()->getParam()->getIntParam("SOLUTION_LIMIT")
              //~ << endl;
      //~ }
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
