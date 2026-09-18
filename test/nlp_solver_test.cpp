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
   LOG_INTER("*** Solving status with NLPSolver:" << status
                                                  << ", expected: " << expectedStatus);
   LOG_INTER("Time spent: " << rp_localopt.timeSpent());
   LOG_INTER("Number of iterations: " << rp_localopt.iterUsed());
   if (status == OptimizationStatus::Optimal)
   {
      LOG_INTER("Best point: " << rp_localopt.bestPoint()
                               << " with obj: " << rp_localopt.bestVal());
   }
   TEST_TRUE(status == expectedStatus);
}

void test_pw()
{
   LOG_INTER(std::endl << "Testing optimizing with piecewise constraints")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-2,3];
        Constraints
          piecewise(x,{
            [-3,0]: -x+y-2 >= 0,
            [0,3]: x+y-1 >= 0});
        MIN (x+1)^2+y^2-2;
       )RP";

   Problem prob;
   Parser parser;
   parser.parseStr(rp_str, prob);
   LOG_INTER("Problem parsed " << prob)
   NLPSolver opt(prob);

   RealPoint sol(prob.scope());
   OptimizationStatus status = OptimizationStatus::Other;
   IntervalBox box(prob.scope());
   RealPoint start(prob.scope(), box.midpoint());
   LOG_INTER("Initial box: " << box)

   status = opt.minimize(box, start);
   LOG_INTER(std::endl << "*** Solving status with NLPSolver:" << status);
   LOG_INTER("Time spent: " << opt.timeSpent());
   LOG_INTER("Number of iterations: " << opt.iterUsed());
   if (status == OptimizationStatus::Optimal)
   {
      LOG_INTER("Best point: " << opt.bestPoint() << " with obj: " << opt.bestVal());
   }
   TEST_TRUE(status == OptimizationStatus::Optimal);

   box.set(prob.varAt(0), Interval(-3, -1e-12));
   RealPoint start2(prob.scope(), box.midpoint());
   status = opt.minimize(box, start2);
   LOG_INTER(std::endl << "*** Solving status with NLPSolver:" << status);
   LOG_INTER("Time spent: " << opt.timeSpent());
   LOG_INTER("Number of iterations: " << opt.iterUsed());
   if (status == OptimizationStatus::Optimal)
   {
      LOG_INTER("Best point: " << opt.bestPoint() << " with obj: " << opt.bestVal());
   }
   TEST_TRUE(status == OptimizationStatus::Optimal);

   box.set(prob.varAt(0), Interval(1e-12, 3));
   RealPoint start3(prob.scope(), box.midpoint());
   status = opt.minimize(box, start3);
   LOG_INTER(std::endl << "*** Solving status with NLPSolver:" << status);
   LOG_INTER("Time spent: " << opt.timeSpent());
   LOG_INTER("Number of iterations: " << opt.iterUsed());
   if (status == OptimizationStatus::Optimal)
   {
      LOG_INTER("Best point: " << opt.bestPoint() << " with obj: " << opt.bestVal());
   }
   TEST_TRUE(status == OptimizationStatus::Optimal);
}

void test_cond()
{
   LOG_INTER(std::endl << "Testing optimizing with a conditional constraint")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-2,3];
        Constraints
            x in [-3,0] -> -x+y-2 >= 0;
        MIN (x+1)^2+y^2-2;
       )RP";

   Problem prob;
   Parser parser;
   parser.parseStr(rp_str, prob);
   LOG_INTER("Problem parsed " << prob)
   NLPSolver opt(prob);

   RealPoint sol(prob.scope());
   OptimizationStatus status = OptimizationStatus::Other;
   IntervalBox box(prob.scope());
   RealPoint start(prob.scope(), box.midpoint());
   LOG_INTER("Initial box: " << box)

   status = opt.minimize(box, start);
   LOG_INTER(std::endl << "*** Solving status with NLPSolver:" << status);
   LOG_INTER("Time spent: " << opt.timeSpent());
   LOG_INTER("Number of iterations: " << opt.iterUsed());
   if (status == OptimizationStatus::Optimal)
   {
      LOG_INTER("Best point: " << opt.bestPoint() << " with obj: " << opt.bestVal());
   }
   TEST_TRUE(status == OptimizationStatus::Optimal);
}

void test_table()
{
   LOG_INTER(std::endl << "Testing optimizing with a table constraint")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-2,3];
        Constraints
          table({x,y},{1,1,2,2,0,1,-1,0});
        MIN (x+1)^2+y^2-2;
       )RP";

   Problem prob;
   Parser parser;
   parser.parseStr(rp_str, prob);
   LOG_INTER("Problem parsed " << prob)
   NLPSolver opt(prob);

   RealPoint sol(prob.scope());
   OptimizationStatus status = OptimizationStatus::Other;
   IntervalBox box(prob.scope());
   RealPoint start(prob.scope(), box.midpoint());
   LOG_INTER("Initial box: " << box)

   status = opt.minimize(box, start);
   LOG_INTER(std::endl << "*** Solving status with NLPSolver:" << status);
   LOG_INTER("Time spent: " << opt.timeSpent());
   LOG_INTER("Number of iterations: " << opt.iterUsed());
   if (status == OptimizationStatus::Optimal)
   {
      LOG_INTER("Best point: " << opt.bestPoint() << " with obj: " << opt.bestVal());
   }
   TEST_TRUE(status == OptimizationStatus::Optimal);
}

int main()
{
   Logger::init(LogLevel::low, "nlp_solver_test.log");

   pb = nullptr;

   test_localopt("../benchmarks/cop/Beale.rp");

   pb = nullptr;

   test_localopt("../benchmarks/cop/Bird.rp");

   pb = nullptr;

   // test_localopt("../benchmarks/cop/OLS.rp", OptimizationStatus::Infeasible);

   // pb = nullptr;

   // test_localopt("../benchmarks/cop/coil_spring.rp", OptimizationStatus::Optimal);

   test_pw();

   test_cond();

   // test_table();

   END_TEST
}
