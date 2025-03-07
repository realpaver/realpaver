#include "test_config.hpp"
#include "realpaver/ContractorHC4Revise.hpp"
#include "realpaver/ContractorList.hpp"
#include "realpaver/ContractorLoop.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"
#include <iomanip>
#include <memory>

void init()
{}

void clean()
{}

void test_1()
{
   Problem prob;
   Variable x = prob.addRealVar(0, 10, "x");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(sqr(x)-x == 0);

   IntervalBox B(prob.scope());
   SharedContractor op = std::make_shared<ContractorHC4Revise>(dag, 0);

   ContractorLoop ctc(op);
   ctc.setTol(1.0e-1);
   TEST_TRUE(ctc.contract(B) == Proof::Maybe);

   Interval fixed_point(0, 1),
            outer(0, 1.075);

   TEST_TRUE(B.get(x).contains(fixed_point));
   TEST_TRUE(outer.contains(B.get(x)));
}

void test_2()
{
   Problem prob;
   Variable x = prob.addRealVar(0, 10, "x");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(sqr(x)-x == 0);

   IntervalBox B(prob.scope());
   SharedContractor op = std::make_shared<ContractorHC4Revise>(dag, 0);

   ContractorLoop ctc(op);
   ctc.setTol(1.0e-2);
   TEST_TRUE(ctc.contract(B) == Proof::Maybe);

   Interval fixed_point(0, 1),
            outer(0, 1.0095);

   TEST_TRUE(B.get(x).contains(fixed_point));
   TEST_TRUE(outer.contains(B.get(x)));
}

void test_3()
{
   Problem prob;
   Variable x = prob.addRealVar(0, 10, "x");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(sqr(x)-x == 0);

   IntervalBox B(prob.scope());
   SharedContractor op = std::make_shared<ContractorHC4Revise>(dag, 0);

   ContractorLoop ctc(op);
   ctc.setTol(1.0e-3);
   TEST_TRUE(ctc.contract(B) == Proof::Maybe);

   Interval fixed_point(0, 1),
            outer(0, 1.0006);

   TEST_TRUE(B.get(x).contains(fixed_point));
   TEST_TRUE(outer.contains(B.get(x)));
}

void test_4()
{
   Problem prob;
   Variable x = prob.addRealVar(1.5, 10, "x");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(sqr(x)-x == 0);

   IntervalBox B(prob.scope());
   SharedContractor op = std::make_shared<ContractorHC4Revise>(dag, 0);

   ContractorLoop ctc(op);
   ctc.setTol(1.0);
   TEST_TRUE(ctc.contract(B) == Proof::Maybe);
}

void test_5()
{
   Problem prob;
   Variable x = prob.addRealVar(1.5, 10, "x");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(sqr(x)-x == 0);

   IntervalBox B(prob.scope());
   SharedContractor op = std::make_shared<ContractorHC4Revise>(dag, 0);

   ContractorLoop ctc(op);
   ctc.setTol(0.5);
   TEST_TRUE(ctc.contract(B) == Proof::Empty);
}

void test_6()
{
   Problem prob;
   Variable x = prob.addRealVar(0, 10, "x"),
            y  = prob.addRealVar(0, 10, "y");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(sqr(x)-y == 0);
   dag->insert(sqr(x)+sqr(y) == 2);

   IntervalBox B(prob.scope());
   SharedContractor h0 = std::make_shared<ContractorHC4Revise>(dag, 0);
   SharedContractor h1 = std::make_shared<ContractorHC4Revise>(dag, 1);
   SharedContractorList op = std::make_shared<ContractorList>();
   op->push(h0);
   op->push(h1);

   ContractorLoop ctc(op);
   ctc.setTol(1.e0);
   TEST_TRUE(ctc.contract(B) == Proof::Maybe);

   Interval outer(0, 1.414214);
   TEST_TRUE(outer.contains(B.get(x)));
   TEST_TRUE(outer.contains(B.get(y)));
}

void test_7()
{
   Problem prob;
   Variable x = prob.addRealVar(0, 10, "x"),
            y  = prob.addRealVar(0, 10, "y");

   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   dag->insert(sqr(x)-y == 0);
   dag->insert(sqr(x)+sqr(y) == 2);

   IntervalBox B(prob.scope());
   SharedContractor h0 = std::make_shared<ContractorHC4Revise>(dag, 0);
   SharedContractor h1 = std::make_shared<ContractorHC4Revise>(dag, 1);
   SharedContractorList op = std::make_shared<ContractorList>();
   op->push(h0);
   op->push(h1);

   ContractorLoop ctc(op);
   ctc.setTol(1.e-1);
   TEST_TRUE(ctc.contract(B) == Proof::Maybe);

   Interval outer(0.99999999, 1.000000001);
   TEST_TRUE(outer.contains(B.get(x)));
   TEST_TRUE(outer.contains(B.get(y)));
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
   TEST(test_7)

   CLEAN_TEST
   END_TEST
}
