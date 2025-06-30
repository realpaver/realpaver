#include "realpaver/ExporterHDF5.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Solver.hpp"
#include <iostream>

using namespace ::realpaver;

int main()
{
   /////////
   std::string pb2_str = R"RP(
     Variables
         x in [-2,2],
         y in [-2,2];
     Constraints
         x^2 + y^2 <= 2;
   )RP";
   Problem prob;
   Parser parser;
   parser.parseStr(pb2_str, prob);
   Solver solver(prob);
   solver.getEnv()->getParams()->setDblParam("TIME_LIMIT", 1);
   solver.getEnv()->getParams()->setDblParam("VAR_ABS_TOL", .01);
   solver.getEnv()->getParams()->setDblParam("SOLUTION_CLUSTER_GAP", 0);
   solver.getEnv()->getParams()->setStrParam("TRACE", "NO");
   solver.getEnv()->getParams()->setStrParam("LOG_LEVEL", "NONE");
   solver.solve();
   std::cout << solver.getSpace()->nbSolNodes() << " solutions and "
             << solver.getSpace()->nbPendingNodes() << " pending nodes" << std::endl;
   ExporterHDF5 hdf5(prob);
   hdf5.save(solver, "test.h5");
   return 0;
}
