#include "test_config.hpp"
#include "test_utils.hpp"

// Basic function to create a linearizer from a problem
std::shared_ptr<Linearizer> get_linearizer(SharedProblem pb, bool hansen = true,
                                           CornerStyle corner = CornerStyle::Left,
                                           std::shared_ptr<Env> e = nullptr)
{
   std::shared_ptr<Env> env = e;
   if (env == nullptr)
      env = std::make_shared<Env>();
   ContractorFactory cf(*pb, env);
   std::shared_ptr<Linearizer> linearizer;
   int goal_ctr_id = cf.getObjectiveConstraintDagId();
   LOG_INTER("OBJ CTR ID: " << goal_ctr_id)
   linearizer = std::make_shared<LinearizerInnerTaylor>(cf.extDag(), goal_ctr_id, hansen,
                                                        CornerStyle::Left);
   return linearizer;
}

// Generic function to build linearizer and run LP solver on it
LPStatus run_linear_relax(SharedProblem pb, IntervalBox &B, double relax_tol = 0.0)
{
   LOG_INTER("Problem: " << std::endl << *pb);
   SharedDagExtended dag = getExtendedDag(pb);

   // Create inner linearizer
   std::shared_ptr<Linearizer> lit = get_linearizer(pb);
   lit->setRelaxTol(relax_tol);
   // Create LP Solver
   LPSolver lp_solver;
   lit->makeLP(lp_solver, B);
   // Add objective to solver
   LinExpr le;
   le.addTerm(1, lp_solver.getLinVar(1));
   lp_solver.setCost(le);
   // Run LP solver
   LPStatus success = lp_solver.optimize();
   if (success == LPStatus::Optimal)
   {
      LOG_MAIN("**** Inner opt val: " << lp_solver.certifiedCostSolution() << " ~ "
                                      << lp_solver.primalSolution())
   }
   else
   {
      LOG_MAIN("Inner relaxation failure: " << success)
   }
   return success;
}

// Basic test with quadratic expression
void test1()
{
   LOG_MAIN("TEST1")
   std::string rp_str = R"RP(
       Variables
         x in [-3.0,3.0],
         y in [-0,10];
       Constraints
         y >= x^2;
      )RP";
   SharedProblem pb = getProblem(rp_str);
   IntervalBox B(pb->scope());
   LPStatus success = run_linear_relax(pb, B);
   TEST_TRUE(success == LPStatus::Optimal)
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
   LPStatus success = run_linear_relax(pb, B);
   TEST_TRUE(success == LPStatus::Optimal)
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
   LPStatus success = run_linear_relax(pb, B);
   TEST_TRUE(success == LPStatus::Optimal)
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
   LPStatus success = run_linear_relax(pb, B);
   TEST_TRUE(success == LPStatus::Optimal)
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
   LPStatus success = run_linear_relax(pb, B);
   TEST_TRUE(success == LPStatus::Optimal)
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
   LPStatus success = run_linear_relax(pb, B, 1e-8);
   TEST_TRUE(success == LPStatus::Infeasible)
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
   LPStatus success = run_linear_relax(pb, B, 1e-8);

   TEST_TRUE(success == LPStatus::Infeasible)
   LOG_MAIN("END TEST PROCESS" << std::endl)
}

// Test of process problem on a small box
void test_process_box()
{
   LOG_MAIN("TEST PROCESS BOX")
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
   // Box:
   // [1728.915405624773, 1728.920909117964] ; [15999.94032134023, 16000] ;
   // [98.1183301474912, 98.16771842458343] ; [3056.482457368713, 3056.492536829011] ;
   // [1999.993192375056, 2000] ; [90.609375, 90.6171875] ;
   // [94.18557599914915, 94.18812833436596] ; [10.41108638330841, 10.41114751730836] ;
   // [2.616706522440267, 2.618406372346661] ; [149.5567280074474, 149.5643850130978]
   IntervalBox B(pb->scope());
   B.set(pb->varAt(0), Interval(1728.915405624773, 1728.920909117964));
   B.set(pb->varAt(1), Interval(15999.94032134023, 16000));
   B.set(pb->varAt(2), Interval(98.1183301474912, 98.16771842458343));
   B.set(pb->varAt(3), Interval(3056.482457368713, 3056.492536829011));
   B.set(pb->varAt(4), Interval(1999.993192375056, 2000));
   B.set(pb->varAt(5), Interval(90.609375, 90.6171875));
   B.set(pb->varAt(6), Interval(94.18557599914915, 94.18812833436596));
   B.set(pb->varAt(7), Interval(10.41108638330841, 10.41114751730836));
   B.set(pb->varAt(8), Interval(2.616706522440267, 2.618406372346661));
   B.set(pb->varAt(9), Interval(149.5567280074474, 149.5643850130978));
   B.set(pb->getObjective().getVar(), pb->getObjective().getTerm().eval(B));

   LPStatus success = run_linear_relax(pb, B);

   TEST_TRUE(success == LPStatus::Optimal)
   LOG_MAIN("END TEST PROCESS BOX" << std::endl)
}

int main()
{
   Logger::init(LogLevel::inter, "inner_taylor_relax_test.log");
   test1();
   test2();
   test3();
   test4();

   test_eq_strict();
   test_eq_relax();

   test_process();
   test_process_box();

   END_TEST
}
