#include "gaol/gaol_interval.h"
#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintProver.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/ProverOptim.hpp"
#include "test_config.hpp"

Problem *prob;
Variable x, y, z;

void init()
{
   prob = new Problem();
   x = prob->addRealVar(0, 0, "x");
   y = prob->addRealVar(0, 0, "y");
   z = prob->addRealVar(0, 0, "z");
   prob->addCtr({x >= y, x <= y, z == x + y, x >= y});
}

void clean()
{
   delete prob;
}

void test_1()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   Bitset active(4);
   active.setOne(0);
   active.setOne(3);

   ProverOptim prover(*prob);
   TEST_TRUE(prover.certify(B, active));
}

void test_2()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   Bitset active(4);
   active.setOne(0);
   active.setOne(1);
   active.setOne(3);

   ProverOptim prover(*prob);
   TEST_FALSE(prover.certify(B, active));
}

void test_3()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   Bitset active(4);
   active.setOne(0);
   active.setOne(2);
   active.setOne(3);

   ProverOptim prover(*prob);
   TEST_FALSE(prover.certify(B, active));
}

void test_4()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   Bitset active(4);
   active.setOne(0);
   active.setOne(2);
   active.setOne(3);

   ProverOptim prover(*prob, 3.5);
   TEST_FALSE(prover.certify(B, active));
}

void test_5()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   Bitset active(4);
   active.setOne(0);
   active.setOne(2);
   active.setOne(3);

   ProverOptim prover(*prob, 4.5);
   TEST_TRUE(prover.certify(B, active));
}

void test_6()
{
   IntervalBox B(prob->scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   prob->ctrAt(1).setInitial(false);

   Bitset active(4);
   active.setOne(0);
   active.setOne(1);

   ProverOptim prover(*prob, 4.5);
   prob->ctrAt(1).setInitial(true);

   TEST_TRUE(prover.certify(B, active));
}

int main()
{
   INIT_TEST

   TEST(test_1)
   TEST(test_2)
   TEST(test_3)
   TEST(test_4)
   TEST(test_5)
   TEST(test_6)

   CLEAN_TEST
   END_TEST
}
