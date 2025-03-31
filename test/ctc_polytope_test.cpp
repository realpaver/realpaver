#include "realpaver/ContractorPolytope.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Linearizer.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"
#include <memory>

void init()
{
}

void clean()
{
}

void test_1()
{
   Problem prob;
   Variable x = prob.addRealVar(0, 0, "x"), y = prob.addRealVar(0, 0, "y");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(y - sqr(x) >= 0);
   dag->insert(y + sqr(x) - 2 <= 0);

   std::unique_ptr<LinearizerTaylor> lzr = std::make_unique<LinearizerTaylor>(dag);

   Bitset corner({0, 1});
   lzr->fixFirstCorner(corner);
   ContractorPolytope ctc(std::move(lzr));

   IntervalBox B(prob.scope());
   B.set(x, Interval(-2, 1));
   B.set(y, Interval(-1, 2));

   Proof proof = ctc.contract(B);
   TEST_TRUE(proof != Proof::Empty);

   Interval resx(-1.25, 1), resy(-1, 2);

   TEST_TRUE(B.get(x).contains(resx));
   TEST_TRUE(resx.distance(B.get(x)) < 1.0e-8);
   TEST_TRUE(B.get(y).isSetEq(resy));

   // relaxation
   // -4 <= 4x + y
   // -1 <= -2x + y
   // -4x + y <= 6
   // 2x + y <= 3
   // -2 <= x <= 1
   // -1 <= y <= 2
}

void test_2()
{
   Problem prob;
   Variable x = prob.addRealVar(0, 0, "x"), y = prob.addRealVar(0, 0, "y");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(y - sqr(x) >= 0);
   dag->insert(y + sqr(x) - 2 <= 0);

   std::unique_ptr<LinearizerTaylor> lzr = std::make_unique<LinearizerTaylor>(dag);

   Bitset corner({1, 0});
   lzr->fixFirstCorner(corner);
   ContractorPolytope ctc(std::move(lzr));

   IntervalBox B(prob.scope());
   B.set(x, Interval(-2, 3));
   B.set(y, Interval(-3, 2));

   Proof proof = ctc.contract(B);
   TEST_TRUE(proof != Proof::Empty);

   Interval I(5), J(3), K(I / J), resx(-1.25, K.right()), resy(-3, 2);

   TEST_TRUE(B.get(x).contains(resx));
   TEST_TRUE(resx.distance(B.get(x)) < 1.0e-8);
   TEST_TRUE(B.get(y).isSetEq(resy));

   // relaxation
   // -9 <= -6x + y
   // -4 <= 4x + y
   // 6x + y <= 11
   // -4x + y <= 6
   // -2 <= x <= 3
   // -3 <= y <= 2
}

void brown_5_feasible()
{
   Problem prob;
   Variable x1 = prob.addRealVar(0, 0, "x1"), x2 = prob.addRealVar(0, 0, "x2"),
            x3 = prob.addRealVar(0, 0, "x3"), x4 = prob.addRealVar(0, 0, "x4"),
            x5 = prob.addRealVar(0, 0, "x5");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(2 * x1 + x2 + x3 + x4 + x5 - 6 == 0);
   dag->insert(x1 + 2 * x2 + x3 + x4 + x5 - 6 == 0);
   dag->insert(x1 + x2 + 2 * x3 + x4 + x5 - 6 == 0);
   dag->insert(x1 + x2 + x3 + 2 * x4 + x5 - 6 == 0);
   dag->insert(x1 * x2 * x3 * x4 * x5 - 1 == 0);

   std::unique_ptr<LinearizerTaylor> lzr = std::make_unique<LinearizerTaylor>(dag);

   Bitset corner({0, 1, 0, 1, 0});
   lzr->fixFirstCorner(corner);
   ContractorPolytope ctc(std::move(lzr));

   IntervalBox B(prob.scope());
   B.set(x1, Interval(-10, 10));
   B.set(x2, Interval(-10, 10));
   B.set(x3, Interval(-10, 10));
   B.set(x4, Interval(-10, 10));
   B.set(x5, Interval(-10, 10));

   Proof proof = ctc.contract(B);
   TEST_TRUE(proof != Proof::Empty);

   Interval I("-0.8", "3.2");
   TEST_TRUE(I.distance(B.get(x1)) < 1.0e-8);
   TEST_TRUE(B.get(x1).contains(I));
   TEST_TRUE(I.distance(B.get(x2)) < 1.0e-8);
   TEST_TRUE(B.get(x2).contains(I));
   TEST_TRUE(I.distance(B.get(x3)) < 1.0e-8);
   TEST_TRUE(B.get(x3).contains(I));
   TEST_TRUE(I.distance(B.get(x4)) < 1.0e-8);
   TEST_TRUE(B.get(x4).contains(I));
   TEST_TRUE(B.get(x5).isSetEq(Interval(-10, 10)));
}

void brown_5_infeasible()
{
   Problem prob;
   Variable x1 = prob.addRealVar(0, 0, "x1"), x2 = prob.addRealVar(0, 0, "x2"),
            x3 = prob.addRealVar(0, 0, "x3"), x4 = prob.addRealVar(0, 0, "x4"),
            x5 = prob.addRealVar(0, 0, "x5");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(2 * x1 + x2 + x3 + x4 + x5 - 6 == 0);
   dag->insert(x1 + 2 * x2 + x3 + x4 + x5 - 6 == 0);
   dag->insert(x1 + x2 + 2 * x3 + x4 + x5 - 6 == 0);
   dag->insert(x1 + x2 + x3 + 2 * x4 + x5 - 6 == 0);
   dag->insert(x1 * x2 * x3 * x4 * x5 - 1 == 0);

   std::unique_ptr<LinearizerTaylor> lzr = std::make_unique<LinearizerTaylor>(dag);

   Bitset corner({0, 1, 0, 1, 0});
   lzr->fixFirstCorner(corner);
   ContractorPolytope ctc(std::move(lzr));

   IntervalBox B(prob.scope());
   B.set(x1, Interval(2, 10));
   B.set(x2, Interval(-1, 1));
   B.set(x3, Interval(-1, 1));
   B.set(x4, Interval(-1, 1));
   B.set(x5, Interval(-1, 1));

   Proof proof = ctc.contract(B);
   TEST_TRUE(proof == Proof::Empty);
}

int main()
{
   INIT_TEST

   TEST(test_1)
   TEST(test_2)
   TEST(brown_5_feasible)
   TEST(brown_5_infeasible)

   CLEAN_TEST
   END_TEST
}
