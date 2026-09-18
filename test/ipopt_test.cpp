#include "realpaver/IntervalBox.hpp"
#include "realpaver/NLPSolverIpopt.hpp"
#include "realpaver/RealPoint.hpp"
#include "test_config.hpp"
#include "test_utils.hpp"

// Basic test with quadratic expression
void test1()
{
   LOG_MAIN("TEST1")
   std::string rp_str = R"RP(
     Variables
        x in [-4.5, 4.5],
        y in [-4.5, 4.5];
     Min
       (1.5 - x + x*y)^2 + (2.25 - x + x*y^2)^2 + (2.625 - x + x*y^3)^2;
     Constraints
         x*y >= 0;
      )RP";
   SharedProblem pb = getProblem(rp_str);
   LOG_INTER(*pb)
   IntervalBox B(pb->scope());
   NLPSolver ipopt(*pb);
   IntervalBox box(pb->scope());
   RealPoint start(box.midpoint());
   OptimizationStatus success = ipopt.minimize(box, start);
   TEST_TRUE(success == OptimizationStatus::Optimal)
   LOG_MAIN("END TEST1" << std::endl)
}

// Basic test with nonlinear (exp) expression
void test2()
{
   LOG_MAIN("TEST2")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10];
        Constraints
          y >= exp(x);
       )RP";
   SharedProblem pb = getProblem(rp_str);
   IntervalBox B(pb->scope());
   NLPSolver ipopt(*pb);
   IntervalBox box(pb->scope());
   RealPoint start(box.midpoint());
   OptimizationStatus success = ipopt.minimize(box, start);
   TEST_TRUE(success ==
             OptimizationStatus::Infeasible) // Too big error term in affine relaxation
   LOG_MAIN("END TEST2" << std::endl)
}

// Basic test with nonlinear (sin) expression
void test3()
{
   LOG_MAIN("TEST3")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10];
        Constraints
          y >= sin(x);
       )RP";
   SharedProblem pb = getProblem(rp_str);
   IntervalBox B(pb->scope());
   NLPSolver ipopt(*pb);
   IntervalBox box(pb->scope());
   RealPoint start(box.midpoint());
   OptimizationStatus success = ipopt.minimize(box, start);
   TEST_TRUE(success == OptimizationStatus::Optimal)
   LOG_MAIN("END TEST3" << std::endl)
}

// Test with 2 nonlinear expressions
void test4()
{
   LOG_MAIN("TEST4")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10],
          z in [0,5];
        Constraints
          z <= y + x^2,
          y >= sin(x);
       )RP";
   SharedProblem pb = getProblem(rp_str);
   IntervalBox B(pb->scope());
   NLPSolver ipopt(*pb);
   IntervalBox box(pb->scope());
   RealPoint start(box.midpoint());
   OptimizationStatus success = ipopt.minimize(box, start);
   TEST_TRUE(success == OptimizationStatus::Optimal)
   LOG_MAIN("END TEST4" << std::endl)
}

// Test with an equality constraint without relaxation: only equality
void test_eq_strict()
{
   LOG_MAIN("TEST EQ STRICT")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10],
          z in [0,5];
        Constraints
          z == y + x^2,
          y >= sin(x);
       )RP";
   SharedProblem pb = getProblem(rp_str);
   IntervalBox B(pb->scope());
   NLPSolver ipopt(*pb);
   IntervalBox box(pb->scope());
   RealPoint start(box.midpoint());
   OptimizationStatus success = ipopt.minimize(box, start);
   TEST_TRUE(success == OptimizationStatus::Optimal)
   LOG_MAIN("END TEST EQ STRICT" << std::endl)
}

// Test with an equality constraint with relaxation to 2 inequalities
void test_eq_relax()
{
   LOG_MAIN("TEST EQ RELAX")
   std::string rp_str = R"RP(
        Variables
          x in [-3.0,3.0],
          y in [-0,10],
          z in [0,5];
        Constraints
          z == y + x^2,
          y >= sin(x);
       )RP";
   SharedProblem pb = getProblem(rp_str);
   IntervalBox B(pb->scope());
   NLPSolver ipopt(*pb);
   IntervalBox box(pb->scope());
   RealPoint start(box.midpoint());
   OptimizationStatus success = ipopt.minimize(box, start);
   TEST_TRUE(success == OptimizationStatus::Infeasible)
   LOG_MAIN("END TEST EQ RELAX" << std::endl)
}

// Test on process problem example
void test_process()
{
   LOG_MAIN("TEST PROCESS")
   std::string rp_str = R"RP(
    Variables
    x1 in [10, 2000],
    x2 in [0, 16000],
    x3 in [0, 120],
    x4 in [0, 5000],
    x5 in [0, 2000],
    x6 in [85, 93],
    x7 in [90, 95],
    x8 in [3, 12],
    x9 in [1.2, 4],
    x10 in [145, 162];

    Min - 0.063*x4*x7 + 5.04*x1 + 0.035*x2 + 10*x3 + 3.36*x5;
    Constraints
    - x1*(1.12 + 0.13167*x8 - 0.00667*x8^2) + x4 == 0,
    - x1 + 1.22*x4 - x5 == 0,
    - 0.001*x4*x9*x6/(98 - x6) + x3 == 0,
    - (1.098*x8 - 0.038*x8^2) - 0.325*x6 + x7 == 57.425,
    - (x2 + x5)/x1 + x8 == 0,
      x9 + 0.222*x10 == 35.82,
    - 3*x7 + x10 == -133;
   )RP";

   SharedProblem pb = getProblem(rp_str);
   IntervalBox B(pb->scope());
   NLPSolver ipopt(*pb);
   IntervalBox box(pb->scope());
   RealPoint start(box.midpoint());
   OptimizationStatus success = ipopt.minimize(box, start);

   TEST_TRUE(success == OptimizationStatus::Infeasible)
   LOG_MAIN("END TEST PROCESS" << std::endl)
}

int main()
{
   Logger::init(LogLevel::low, "ipopt_test.log");
   test1();
   // test2();
   // test3();
   // test4();

   // test_eq_strict();
   // test_eq_relax();

   // test_process();

   END_TEST
}
