#include "realpaver/LPSolver.hpp"
#include "test_config.hpp"
#include <vector>

void test_optimal_1()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // x1 + 2x2 <= 14
   LinExpr e1 = {{1.0, 2.0}, {x1, x2}};
   solver.addCtr(e1, 14.0);

   // 3x1 - x2 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(e3, 2.0);

   // maximize 3x1 + 4x2
   LinExpr eo = {{3.0, 4.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (6, 4)
   //           f* = 34

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 34.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_optimal_safe_1()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // x1 + 2x2 <= 14
   LinExpr e1 = {{1.0, 2.0}, {x1, x2}};
   solver.addCtr(e1, 14.0);

   // 3x1 - x2 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(e3, 2.0);

   // maximize 3x1 + 4x2
   LinExpr eo = {{3.0, 4.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (6, 4)
   //           y* = (7/3, 0, 1/3)
   //           f* = 34

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Optimal);

   double sol = solver.certifiedCostSolution();

   Interval err(0.0, 1.0e-10), res = 34.0 + err;

   TEST_TRUE(res.contains(sol));
}

void test_optimal_2()
{
   LPSolver solver;
   double inf = Double::inf();

   LinVar x1 = solver.makeVar(0.0, inf, "x1"), x2 = solver.makeVar(0.0, inf, "x2"),
          x3 = solver.makeVar(0.0, inf, "x3");

   // 2x1 + 3x2 + x3 <= 5
   LinExpr e1 = {{2.0, 3.0, 1.0}, {x1, x2, x3}};
   solver.addCtr(e1, 5.0);

   // 4x1 + x2 + 2x3 <= 11
   LinExpr e2 = {{4.0, 1.0, 2.0}, {x1, x2, x3}};
   solver.addCtr(e2, 11.0);

   // 3x1 + 4x2 + 2x3 <= 8
   LinExpr e3 = {{3.0, 4.0, 2.0}, {x1, x2, x3}};
   solver.addCtr(e3, 8.0);

   // maximize 5x1 + 4x2 + 3x3
   LinExpr eo = {{5.0, 4.0, 3.0}, {x1, x2, x3}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (2, 0, 1)
   //           f* =13

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 13.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_optimal_safe_2()
{
   LPSolver solver;

   // reasonable upper bound necessary for proof making
   double ub = 1.0e3;

   LinVar x1 = solver.makeVar(0.0, ub, "x1"), x2 = solver.makeVar(0.0, ub, "x2"),
          x3 = solver.makeVar(0.0, ub, "x3");

   // 2x1 + 3x2 + x3 <= 5
   LinExpr e1 = {{2.0, 3.0, 1.0}, {x1, x2, x3}};
   solver.addCtr(e1, 5.0);

   // 4x1 + x2 + 2x3 <= 11
   LinExpr e2 = {{4.0, 1.0, 2.0}, {x1, x2, x3}};
   solver.addCtr(e2, 11.0);

   // 3x1 + 4x2 + 2x3 <= 8
   LinExpr e3 = {{3.0, 4.0, 2.0}, {x1, x2, x3}};
   solver.addCtr(e3, 8.0);

   // maximize 5x1 + 4x2 + 3x3
   LinExpr eo = {{5.0, 4.0, 3.0}, {x1, x2, x3}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (2, 0, 1)
   //           f* = 13

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Optimal);

   double sol = solver.certifiedCostSolution();

   Interval err(0.0, 1.0e-10), res = 13.0 + err;

   TEST_TRUE(res.contains(sol));
}

void test_bound_ctr()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 5.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // x1 + 2x2 <= 14
   LinExpr e1 = {{1.0, 2.0}, {x1, x2}};
   solver.addCtr(e1, 14.0);

   // 3x1 - x2 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(e3, 2.0);

   // maximize 3x1 + 4x2
   LinExpr eo = {{3.0, 4.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (5, 4.5)
   //           f* = 33
   // the bound constraint x1 = 5 is active

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 33.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_parallel_obj()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // x1 + 2x2 <= 14
   LinExpr e1 = {{1.0, 2.0}, {x1, x2}};
   solver.addCtr(e1, 14.0);

   // 3x1 - x2 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(e3, 2.0);

   // maximize x1 + 2x2
   // the objective is parallel to the first constraint
   // there are an infinity of maximizers
   LinExpr eo = {{1.0, 2.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (2, 6) (for example)
   //           f* = 14
   // the bound constraint x1 = 5 is active

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 14.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_quasi_parallel_obj()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // x1 + 2x2 <= 14
   LinExpr e1 = {{1.0, 2.0}, {x1, x2}};
   solver.addCtr(e1, 14.0);

   // 3x1 - x2 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(e3, 2.0);

   // maximize 0.495x1 + x2
   // the objective is quasi parallel to the first constraint
   LinExpr eo = {{0.495, 1.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (2, 6)
   //           f* = 14

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
}

void test_unbounded()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, Double::inf(), "x1"),
          x2 = solver.makeVar(0.0, Double::inf(), "x2");

   // x1 + 2x2 <= 14
   // LinExpr e1 = { {1.0, 2.0}, {x1, x2} };
   // solver.addCtr(e1, 14.0);
   // the problem is unbounded without this constraint

   // 3x1 - x2 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(e3, 2.0);

   // maximize x1 + 2x2
   LinExpr eo = {{1.0, 2.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Unbounded);
}

void test_infeasible_1()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // 3x1 - x2 <= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(e2, 0.0);

   // x1 - x2 >= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(2.0, e3);

   // maximize x1 + 2x2
   LinExpr eo = {{1.0, 2.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Infeasible);
}

void test_infeasible_2()
{
   LPSolver solver;

   double inf = Double::inf();

   LinVar x1 = solver.makeVar(-inf, inf, "x1"), x2 = solver.makeVar(-inf, inf, "x2"),
          x3 = solver.makeVar(0.0, inf, "x2"), x4 = solver.makeVar(0.0, inf, "x2");

   // x1 + 3x2 + 2x3 + 4x4 <= 5
   LinExpr e1 = {{1.0, 3.0, 2.0, 4.0}, {x1, x2, x3, x4}};
   solver.addCtr(e1, 5.0);

   // 3x1 + x2 + 2x3 + x4 <= 4
   LinExpr e2 = {{3.0, 1.0, 2.0, 1.0}, {x1, x2, x3, x4}};
   solver.addCtr(e2, 4.0);

   // 5x1 + 3x2 + 3x3 + 3x4 = 9
   LinExpr e3 = {{5.0, 3.0, 3.0, 3.0}, {x1, x2, x3, x4}};
   solver.addCtr(9.0, e3, 9.0);

   // maximize x1
   LinExpr eo = {{1.0}, {x1}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Infeasible);
}

void test_safe_infeasible_1()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // 3x1 - x2 <= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(e2, 0.0);

   // x1 - x2 >= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(2.0, e3);

   // maximize x1 + 2x2
   LinExpr eo = {{1.0, 2.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Infeasible);
   TEST_TRUE(solver.isCertifiedInfeasible());
}

void test_safe_infeasible_2()
{
   LPSolver solver;

   double inf = Double::inf();

   LinVar x1 = solver.makeVar(-inf, inf, "x1"), x2 = solver.makeVar(-inf, inf, "x2"),
          x3 = solver.makeVar(0.0, inf, "x2"), x4 = solver.makeVar(0.0, inf, "x2");

   // x1 + 3x2 + 2x3 + 4x4 <= 5
   LinExpr e1 = {{1.0, 3.0, 2.0, 4.0}, {x1, x2, x3, x4}};
   solver.addCtr(e1, 5.0);

   // 3x1 + x2 + 2x3 + x4 <= 4
   LinExpr e2 = {{3.0, 1.0, 2.0, 1.0}, {x1, x2, x3, x4}};
   solver.addCtr(e2, 4.0);

   // 5x1 + 3x2 + 3x3 + 3x4 = 9
   LinExpr e3 = {{5.0, 3.0, 3.0, 3.0}, {x1, x2, x3, x4}};
   solver.addCtr(9.0, e3, 9.0);

   // maximize x1
   LinExpr eo = {{1.0}, {x1}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // solving
   LPStatus status = solver.optimize();
   TEST_TRUE(status == LPStatus::Infeasible);
   TEST_TRUE(solver.isCertifiedInfeasible());
}

void KleeMinty(LPSolver &solver, int n)
{
   // variables
   std::vector<LinVar> v;
   for (int i = 1; i <= n; ++i)
      v.push_back(solver.makeVar(0.0, Double::inf()));

   // constraints
   for (int i = 1; i <= n; ++i)
   {
      LinExpr e;
      for (int j = 1; j <= i - 1; ++j)
         e.addTerm(2.0 * ::pow(10.0, i - j), v[j - 1]);

      e.addTerm(1.0, v[i - 1]);
      solver.addCtr(e, ::pow(100.0, i - 1));
   }

   // objective function
   LinExpr eo;
   for (int i = 1; i <= n; ++i)
      eo.addTerm(::pow(10.0, n - i), v[i - 1]);

   solver.setCost(eo);
   solver.setSense(LPSense::Max);
}

void test_klee_minty_4()
{
   LPSolver solver;
   KleeMinty(solver, 4);

   // solving
   solver.setMaxIter(20);
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
}

void test_klee_minty_8()
{
   LPSolver solver;
   KleeMinty(solver, 8);

   // solving
   solver.setMaxIter(255);
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
}

void butterfly(LPSolver &solver, int d)
{
   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   double eps = ::pow(2.0, d);

   // x2 + (1+eps)x1 <= 2+eps
   LinExpr e1 = {{1.0 + eps, 1.0}, {x1, x2}};
   solver.addCtr(e1, 2.0 + eps);

   // x2 + (1-eps)x1 <= 2-eps
   LinExpr e2 = {{1.0 - eps, 1.0}, {x1, x2}};
   solver.addCtr(e2, 2.0 - eps);

   // maximize x1 + x2
   LinExpr eo = {{1.0, 1.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (1, 1)
   //           f* = 2
}

void test_butterfly_1()
{
   LPSolver solver;
   butterfly(solver, -4);

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 2.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_butterfly_2()
{
   LPSolver solver;
   butterfly(solver, -8);

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 2.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_butterfly_3()
{
   LPSolver solver;
   butterfly(solver, -12);

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 2.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_butterfly_4()
{
   LPSolver solver;
   butterfly(solver, -16);

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = 2.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_ill_cond()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   double eps = ::pow(2.0, -12);

   // x1 + x2 = 2
   LinExpr e1 = {{1.0, 1.0}, {x1, x2}};
   solver.addCtr(2.0, e1, 2.0);

   // x1 + (1+eps)x2 = 2+eps
   LinExpr e2 = {{1.0, 1.0 + eps}, {x1, x2}};
   solver.addCtr(2.0 + eps, e2, 2.0 + eps);

   // SOLUTION of the system of equations : x* = (1, 1)

   // maximize 3*x1 - x2
   LinExpr eo = {{3.0, -1.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // solving
   LPStatus status = solver.optimize();

   TEST_TRUE(status == LPStatus::Optimal);

   Interval err(-1.0e-12, 1.0e-12), res = 2.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_reoptimize()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(0.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 10.0, "x2");

   // x1 + 2x2 <= 14
   LinExpr e1 = {{1.0, 2.0}, {x1, x2}};
   solver.addCtr(e1, 14.0);

   // 3x1 - x2 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x1, x2}};
   solver.addCtr(0.0, e2);

   // x1 - x2 <= 2
   LinExpr e3 = {{1.0, -1.0}, {x1, x2}};
   solver.addCtr(e3, 2.0);

   // maximize 3x1 + 4x2
   LinExpr eo = {{3.0, 4.0}, {x1, x2}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (6, 4)
   //           f* = 34

   // solving
   LPStatus status = solver.optimize();

   // now change the cost
   // minimize 0.25x1 - x2
   LinExpr eo2 = {{0.25, -1.0}, {x1, x2}};
   solver.setCost(eo2);
   solver.setSense(LPSense::Min);

   // SOLUTION: x* = (2, 6)
   //           f* = -5.5

   // solving
   status = solver.reoptimize();

   TEST_TRUE(status == LPStatus::Optimal);
   Interval err(-1.0e-12, 1.0e-12), res = -5.5 + err;
   TEST_TRUE(res.contains(solver.costSolution()));

   // now change the cost
   // minimize x1
   LinExpr eo3 = {{1.0}, {x1}};
   solver.setCost(eo3);
   solver.setSense(LPSense::Min);

   // SOLUTION: x* = (0, 0)
   //           f* = 0

   // solving
   status = solver.reoptimize();

   TEST_TRUE(status == LPStatus::Optimal);
   res = 0.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));

   // now change the cost
   // maximize x2
   LinExpr eo4 = {{1.0}, {x2}};
   solver.setCost(eo4);
   solver.setSense(LPSense::Max);

   // SOLUTION: x* = (2, 6)
   //           f* = 6

   // solving
   status = solver.reoptimize();

   TEST_TRUE(status == LPStatus::Optimal);
   res = 6.0 + err;
   TEST_TRUE(res.contains(solver.costSolution()));
}

void test_model()
{
   LPSolver solver;

   LinVar x1 = solver.makeVar(2.0, 10.0, "x1"), x2 = solver.makeVar(0.0, 8.0, "x2"),
          x3 = solver.makeVar(-1.0, 5.0, "x3");

   // x3 + 2x1 <= 14
   LinExpr e1 = {{1.0, 2.0}, {x3, x1}};
   solver.addCtr(e1, 14.0);

   // 3x2 - x1 >= 0
   LinExpr e2 = {{3.0, -1.0}, {x2, x1}};
   solver.addCtr(0.0, e2);

   // x1 - x3 + 3x2 <= 2
   LinExpr e3 = {{1.0, -1.0, 3.0}, {x1, x3, x2}};
   solver.addCtr(e3, 2.0);

   // maximize 3x3 - 4x1
   LinExpr eo = {{3.0, -4.0}, {x3, x1}};
   solver.setCost(eo);
   solver.setSense(LPSense::Max);

   IntervalMatrix M = solver.matrix();
   TEST_TRUE(M.ncols() == 3);
   TEST_TRUE(M.nrows() == 6);

   TEST_TRUE(M(0, 0).isSetEq(Interval(1.0)));
   TEST_TRUE(M(0, 1).isSetEq(Interval(0.0)));
   TEST_TRUE(M(0, 2).isSetEq(Interval(0.0)));

   TEST_TRUE(M(1, 0).isSetEq(Interval(0.0)));
   TEST_TRUE(M(1, 1).isSetEq(Interval(1.0)));
   TEST_TRUE(M(1, 2).isSetEq(Interval(0.0)));

   TEST_TRUE(M(2, 0).isSetEq(Interval(0.0)));
   TEST_TRUE(M(2, 1).isSetEq(Interval(0.0)));
   TEST_TRUE(M(2, 2).isSetEq(Interval(1.0)));

   TEST_TRUE(M(3, 0).isSetEq(Interval(2.0)));
   TEST_TRUE(M(3, 1).isSetEq(Interval(0.0)));
   TEST_TRUE(M(3, 2).isSetEq(Interval(1.0)));

   TEST_TRUE(M(4, 0).isSetEq(Interval(-1.0)));
   TEST_TRUE(M(4, 1).isSetEq(Interval(3.0)));
   TEST_TRUE(M(4, 2).isSetEq(Interval(0.0)));

   TEST_TRUE(M(5, 0).isSetEq(Interval(1.0)));
   TEST_TRUE(M(5, 1).isSetEq(Interval(3.0)));
   TEST_TRUE(M(5, 2).isSetEq(Interval(-1.0)));

   IntervalVector C = solver.cost(), Z = {Interval(-4.0), Interval(0.0), Interval(3.0)};
   TEST_TRUE(C.equals(Z));

   IntervalVector X = solver.varBounds(),
                  U = {Interval(2, 10), Interval(0, 8), Interval(-1, 5)};
   TEST_TRUE(X.equals(U));

   IntervalVector LhsRhs = solver.ctrBounds(),
                  V = {Interval::lessThan(14), Interval::positive(),
                       Interval::lessThan(2)};
   TEST_TRUE(LhsRhs.equals(V));

   IntervalVector B = solver.bounds(), W = {Interval(2, 10),      Interval(0, 8),
                                            Interval(-1, 5),      Interval::lessThan(14),
                                            Interval::positive(), Interval::lessThan(2)};
   TEST_TRUE(B.equals(W));
}

int main()
{
   TEST(test_optimal_1)
   TEST(test_optimal_2)
   TEST(test_optimal_safe_1)
   TEST(test_optimal_safe_2)
   TEST(test_bound_ctr)
   TEST(test_parallel_obj)
   TEST(test_quasi_parallel_obj)
   TEST(test_unbounded)
   TEST(test_infeasible_1)
   TEST(test_infeasible_2)
   TEST(test_safe_infeasible_1)
   TEST(test_safe_infeasible_2)
   TEST(test_klee_minty_4)
   TEST(test_klee_minty_8)
   TEST(test_butterfly_1)
   TEST(test_butterfly_2)
   TEST(test_butterfly_3)
   TEST(test_butterfly_4)
   TEST(test_ill_cond)
   TEST(test_reoptimize)
   TEST(test_model)

   END_TEST
}
