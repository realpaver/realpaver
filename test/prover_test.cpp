#include "gaol/gaol_interval.h"
#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintProver.hpp"
#include "realpaver/DagExtended.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Prover.hpp"
#include "test_config.hpp"

void test_eq_square()
{
   Problem pbm;
   Variable x = pbm.addRealVar(0.5, 1.2, "x"), y = pbm.addRealVar(0.8, 1.9, "y");
   pbm.addCtr(sqr(x) + sqr(y) - 2.0 == 0.0);
   pbm.addCtr(y - sqr(x) == 0.0);

   SharedDagExtended edag = std::make_shared<DagExtended>(pbm);
   Prover prover(edag);

   IntervalBox B(pbm.scope());
   Proof proof = prover.certify(B);

   TEST_TRUE(proof == Proof::Feasible);
}

void test_active_1()
{
   Problem pbm;
   Variable x = pbm.addRealVar(0, 0, "x"), y = pbm.addRealVar(0, 0, "y"),
            z = pbm.addRealVar(0, 0, "z");
   pbm.addCtr({x >= y, x <= y, z == x + y, x >= y});

   IntervalBox B(pbm.scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   Bitset active(4);
   active.setOne(0);
   active.setOne(3);

   SharedDagExtended edag = std::make_shared<DagExtended>(pbm);
   Prover prover(edag);
   TEST_TRUE(prover.certify(B, active) == Proof::Inner);
}

void test_active_2()
{
   Problem pbm;
   Variable x = pbm.addRealVar(0, 0, "x"), y = pbm.addRealVar(0, 0, "y"),
            z = pbm.addRealVar(0, 0, "z");
   pbm.addCtr({x >= y, x <= y, z == x + y, x >= y});

   IntervalBox B(pbm.scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   Bitset active(4);
   active.setOne(0);
   active.setOne(1);
   active.setOne(3);

   SharedDagExtended edag = std::make_shared<DagExtended>(pbm);
   Prover prover(edag);
   TEST_TRUE(prover.certify(B, active) == Proof::Maybe);
}

void test_active_3()
{
   Problem pbm;
   Variable x = pbm.addRealVar(0, 0, "x"), y = pbm.addRealVar(0, 0, "y"),
            z = pbm.addRealVar(0, 0, "z");
   pbm.addCtr({x >= y, x <= y, z == x + y, x >= y});

   IntervalBox B(pbm.scope());
   B.set(x, Interval(5, 5));
   B.set(y, Interval(2, 2));
   B.set(z, Interval(6, 7.5));

   Bitset active(4);
   active.setOne(0);
   active.setOne(2);
   active.setOne(3);

   SharedDagExtended edag = std::make_shared<DagExtended>(pbm);
   Prover prover(edag);
   TEST_TRUE(prover.certify(B, active) == Proof::Feasible);
}

void test_active_4()
{
   Problem pbm;
   Variable x = pbm.addRealVar(0, 0, "x"), y = pbm.addRealVar(0, 0, "y"),
            z = pbm.addRealVar(0, 0, "z");
   pbm.addCtr({x >= y, x <= y, z == x + y, x >= y});

   IntervalBox B(pbm.scope());
   B.set(x, Interval(5, 5));
   B.set(y, Interval(2, 2));
   B.set(z, Interval(8, 9.5));

   Bitset active(4);
   active.setOne(0);
   active.setOne(2);
   active.setOne(3);

   SharedDagExtended edag = std::make_shared<DagExtended>(pbm);
   Prover prover(edag);
   prover.setInitBox(B);
   TEST_TRUE(prover.certify(B, active) == Proof::Maybe);
}

void test_active_5()
{
   Problem pbm;
   Variable x = pbm.addRealVar(0, 0, "x"), y = pbm.addRealVar(0, 0, "y"),
            z = pbm.addRealVar(0, 0, "z");
   pbm.addCtr({x >= y, x <= y, z == x + y, x >= y});

   IntervalBox B(pbm.scope());
   B.set(x, Interval(5, 7));
   B.set(y, Interval(2, 3));
   B.set(z, Interval(8, 11));

   pbm.ctrAt(1).setStatus(ConstraintStatus::Redundant);

   Bitset active(4);
   active.setOne(0);
   active.setOne(1);

   SharedDagExtended edag = std::make_shared<DagExtended>(pbm);
   Prover prover(edag);
   TEST_TRUE(prover.certify(B, active) == Proof::Inner);
}

int main()
{
   TEST(test_eq_square)
   TEST(test_active_1)
   TEST(test_active_2)
   TEST(test_active_3)
   TEST(test_active_4)
   TEST(test_active_5)

   END_TEST
}
