#include "realpaver/Logger.hpp"
#include "realpaver/NLPSolver.hpp"
#include "realpaver/Parser.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"

using namespace realpaver;

#include <filesystem>

std::shared_ptr<Problem> pb;

bool init_pointers(std::string filepath)
{
   if (pb == nullptr)
   {
      pb = std::make_shared<Problem>(Problem(filepath));
      Parser parser;
      std::string fullpath = std::filesystem::current_path().string() + "/" + filepath;
      if (!parser.parseFile(fullpath, *pb))
      {
         std::cerr << "Unable to parse file " << fullpath << std::endl;
         std::cerr << parser.getParseError() << std::endl;
         return false;
      }
   }
   return true;
}

void test_localopt(std::string filepath,
                   OptimizationStatus expectedStatus = OptimizationStatus::Optimal)
{
   LOG_INTER("\n*** Solving " << filepath);

   if (!init_pointers(filepath))
   {
      TEST_TRUE(false);
   }

   NLPSolver rp_localopt(*pb);

   RealPoint sol(pb->scope());
   OptimizationStatus status = OptimizationStatus::Other;
   IntervalBox box(pb->scope());
   RealPoint start(pb->scope(), box.midpoint());

   LOG_INTER("Number of variables: " << rp_localopt.nbVars());
   LOG_INTER("Number of constraints: " << rp_localopt.nbCtrs());
   LOG_INTER("Search space: " << box);
   LOG_INTER("Starting point: " << start);

   status = rp_localopt.minimize(box, start);
   LOG_INTER("*** Solving status with NLPSolver:" << status);
   LOG_INTER("Time spent: " << rp_localopt.timeSpent());
   LOG_INTER("Number of iterations: " << rp_localopt.iterUsed());
   if (status == OptimizationStatus::Optimal)
   {
      LOG_INTER("Best point: " << rp_localopt.bestPoint()
                               << " with obj: " << rp_localopt.bestVal());
   }
   TEST_TRUE(status == expectedStatus);
}

int main()
{
   Logger::init(LogLevel::inter, "nlp_solver_test.log");

   pb = nullptr;

   test_localopt("../benchmarks/cop/Beale.rp");

   pb = nullptr;

   test_localopt("../benchmarks/cop/Bird.rp");

   pb = nullptr;

   test_localopt("../benchmarks/cop/OLS.rp", OptimizationStatus::Other);

   pb = nullptr;

   test_localopt("../benchmarks/cop/coil_spring.rp", OptimizationStatus::Other);

   END_TEST
}
