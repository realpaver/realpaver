#include "realpaver/LPSolver.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/RelaxationRLT.hpp"
#include "test_config.hpp"

void init()
{
}

void clean()
{
}

void test_under_convex_1()
{
   // square function and its derivative
   auto f = [](const Interval &x) { return sqr(x); };
   auto df = [](const Interval &x) { return 2.0 * x; };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // under-estimator at m => y >= x - 0.25
   double a = 0.0, b = 1.0, m = 0.5;
   underConvex(solver, iy, ix, a, b, m, f, df);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(c.getLB() == -0.25);
   TEST_TRUE(Double::isInf(c.getUB()));

   LinExpr e({1.0, -1.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

void test_under_convex_2()
{
   // square function and its derivative
   auto f = [](const Interval &x) { return sqr(x); };
   auto df = [](const Interval &x) { return 2.0 * x; };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // under-estimator at m => y >= 2x - 1
   double a = 0.0, b = 1.0, m = 1.0;
   underConvex(solver, iy, ix, a, b, m, f, df);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(c.getLB() == -1.0);
   TEST_TRUE(Double::isInf(c.getUB()));

   LinExpr e({1.0, -2.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

void test_under_convex_3()
{
   // square function and its derivative
   auto f = [](const Interval &x) { return sqr(x); };
   auto df = [](const Interval &x) { return 2.0 * x; };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // under-estimator at m => no constraint since the derivative is null
   double a = 0.0, b = 1.0, m = 0.0;
   underConvex(solver, iy, ix, a, b, m, f, df);

   TEST_TRUE(solver.getNbLinCtrs() == 0);
}

void test_over_convex()
{
   // square function
   auto f = [](const Interval &x) { return sqr(x); };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // over-estimator => y <= x
   double a = 0.0, b = 1.0;
   overConvex(solver, iy, ix, a, b, f);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(Double::isInf(c.getLB()));
   TEST_TRUE(c.getUB() == 0.0);

   LinExpr e({1.0, -1.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

void test_over_concave_1()
{
   // opposite of square function and its derivative
   auto f = [](const Interval &x) { return -sqr(x); };
   auto df = [](const Interval &x) { return -2.0 * x; };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // over-estimator at m => y <= -x + 0.25
   double a = 0.0, b = 1.0, m = 0.5;
   overConcave(solver, iy, ix, a, b, m, f, df);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(Double::isInf(c.getLB()));
   TEST_TRUE(c.getUB() == 0.25);

   LinExpr e({1.0, 1.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

void test_over_concave_2()
{
   // opposite of square function and its derivative
   auto f = [](const Interval &x) { return -sqr(x); };
   auto df = [](const Interval &x) { return -2.0 * x; };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // over-estimator at m => y <= -x + 0.25
   double a = 0.0, b = 1.0, m = 1.0;
   overConcave(solver, iy, ix, a, b, m, f, df);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(Double::isInf(c.getLB()));
   TEST_TRUE(c.getUB() == 1.0);

   LinExpr e({1.0, 2.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

void test_over_concave_3()
{
   // opposite of square function and its derivative
   auto f = [](const Interval &x) { return -sqr(x); };
   auto df = [](const Interval &x) { return -2.0 * x; };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // over-estimator at m => no constraint since the derivative is null
   double a = 0.0, b = 1.0, m = 0.0;
   overConcave(solver, iy, ix, a, b, m, f, df);

   TEST_TRUE(solver.getNbLinCtrs() == 0);
}

void test_under_concave()
{
   // opposite of square function
   auto f = [](const Interval &x) { return -sqr(x); };

   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();

   // over-estimator => y <= x
   double a = 0.0, b = 1.0;
   overConvex(solver, iy, ix, a, b, f);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(Double::isInf(c.getLB()));
   TEST_TRUE(c.getUB() == 0.0);

   LinExpr e({1.0, 1.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

void test_underline()
{
   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();
   double x1 = 0.0, y1 = 1.0, x2 = 1.0, y2 = 3.0;

   // estimator = y >= 2*x + 1
   underLine(solver, iy, ix, x1, y1, x2, y2);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(c.getLB() == 1.0);
   TEST_TRUE(Double::isInf(c.getUB()));

   LinExpr e({1.0, -2.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

void test_overline()
{
   LPSolver solver;
   LinVar x = solver.makeVar(0.0, 1.0, "x"), y = solver.makeVar(0.0, 1.0, "y");
   size_t ix = x.getIndex(), iy = y.getIndex();
   double x1 = 0.0, y1 = 1.0, x2 = 1.0, y2 = 3.0;

   // estimator = y <= 2*x + 1
   overLine(solver, iy, ix, x1, y1, x2, y2);
   LinCtr c = solver.getLinCtr(0);

   TEST_TRUE(Double::isInf(c.getLB()));
   TEST_TRUE(c.getUB() == 1.0);

   LinExpr e({1.0, -2.0}, {y, x});
   TEST_TRUE(e.equals(c.getExpr()));
}

int main()
{
   INIT_TEST

   TEST(test_under_convex_1)
   TEST(test_under_convex_2)
   TEST(test_under_convex_3)
   TEST(test_over_convex)
   TEST(test_over_concave_1)
   TEST(test_over_concave_2)
   TEST(test_over_concave_3)
   TEST(test_under_concave)
   TEST(test_underline)
   TEST(test_overline)

   CLEAN_TEST
   END_TEST
}
