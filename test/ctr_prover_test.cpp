#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintProver.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"

Problem *prob;
Variable x, y, z, z1, z2;

void init()
{
   prob = new Problem();
   x = prob->addRealVar(0, 0, "x");
   y = prob->addRealVar(0, 0, "y");
   z = prob->addRealVar(0, 0, "z");
   z1 = prob->addBinaryVar("z1");
   z2 = prob->addBinaryVar("z2");
}

void clean()
{
   delete prob;
}

void test_ge_1()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 2));
   B.set(y, Interval(3, 4));
   Constraint c = y - x >= 0.0;
   ConstraintProver prover(B);
   c.acceptVisitor(prover);
   TEST_TRUE(prover.isFeasible());
}

void test_ge_2()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 3.5));
   B.set(y, Interval(3, 4));
   Constraint c = y - x >= 0.0;
   ConstraintProver prover(B);
   c.acceptVisitor(prover);
   TEST_FALSE(prover.isFeasible());
}

void test_eq_1()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   Constraint c = y - x == 0.0;
   double tol = 0.5;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_TRUE(prover.isFeasible());
}

void test_eq_2()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   Constraint c = y - x == 0.0;
   double tol = 0.25;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_TRUE(prover.isFeasible());
}

void test_eq_3()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   Constraint c = y - x == 0.0;
   double tol = 0.125;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_FALSE(prover.isFeasible());
}

void test_cond_1()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   B.set(z, Interval(2, 3));
   Constraint c = cond(z <= 1.0, x - y == 0);
   ConstraintProver prover(B);
   c.acceptVisitor(prover);
   TEST_TRUE(prover.isFeasible());
}

void test_cond_2()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   B.set(z, Interval(0, 2));
   Constraint c = cond(z <= 1.0, x - y == 0);
   ConstraintProver prover(B);
   c.acceptVisitor(prover);
   TEST_FALSE(prover.isFeasible());
}

void test_cond_3()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   B.set(z, Interval(0, 1));
   Constraint c = cond(z <= 1.0, x - y == 0);
   double tol = 0.25;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_TRUE(prover.isFeasible());
}

void test_cond_4()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   B.set(z, Interval(0, 1));
   Constraint c = cond(z <= 1.0, x - y == 0);
   double tol = 0.275;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_FALSE(prover.isFeasible());
}

void test_piece_1()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   B.set(z, Interval(0, 3));
   B.set(z1, Interval(0, 1));
   B.set(z2, Interval(0, 1));

   Constraint c = piecewise(z, {z1, z2}, {Interval(1, 2), Interval(3, 4)},
                            {y - x >= 0.0, y - x == 0.0});

   double tol = 0.275;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_FALSE(prover.isFeasible());
}

void test_piece_2()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 2));
   B.set(y, Interval(4, 5));
   B.set(z, Interval(1.25, 1.5));
   B.set(z1, Interval(1.0));
   B.set(z2, Interval(0.0));
   Constraint c = piecewise(z, {z1, z2}, {Interval(1, 2), Interval(3, 4)},
                            {y - x >= 0.0, y - x == 0.0});
   ConstraintProver prover(B);
   c.acceptVisitor(prover);
   TEST_TRUE(prover.isFeasible());
}

void test_piece_3()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   B.set(z, Interval(3.25, 3.5));
   B.set(z1, Interval(0.0));
   B.set(z2, Interval(1.0));
   Constraint c = piecewise(z, {z1, z2}, {Interval(1, 2), Interval(3, 4)},
                            {y - x >= 0.0, y - x == 0.0});
   double tol = 0.125;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_FALSE(prover.isFeasible());
}

void test_piece_4()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(1, 1.25));
   B.set(y, Interval(1, 1.25));
   B.set(z, Interval(3.25, 3.5));
   B.set(z1, Interval(0.0));
   B.set(z2, Interval(1.0));
   Constraint c = piecewise(z, {z1, z2}, {Interval(1, 2), Interval(3, 4)},
                            {y - x >= 0.0, y - x == 0.0});
   double tol = 0.25;
   ConstraintProver prover(B, tol);
   c.acceptVisitor(prover);
   TEST_TRUE(prover.isFeasible());
}

int main()
{
   INIT_TEST

   TEST(test_ge_1)
   TEST(test_ge_2)
   TEST(test_eq_1)
   TEST(test_eq_2)
   TEST(test_eq_3)
   TEST(test_cond_1)
   TEST(test_cond_2)
   TEST(test_cond_3)
   TEST(test_piece_1)
   TEST(test_piece_2)
   TEST(test_piece_3)
   TEST(test_piece_4)

   CLEAN_TEST
   END_TEST
}
