#include "test_config.hpp"
#include "realpaver/ContractorVarCID.hpp"
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/Problem.hpp"

#include <memory>

using namespace std;

Problem* P;
Variable x, y;
ContractorVarCID* cid;
IntervalBox* B;

void init()
{
   P = new Problem();
   x = P->addRealVar(0, 1, "x");
   y = P->addRealVar(0, 1, "y");

   B = new IntervalBox(P->scope());
   Constraint c( x - y == 0 );

   shared_ptr<Contractor> ctc = make_shared<ContractorConstraint>(c);
   unique_ptr<IntervalSlicer> slicer = make_unique<IntervalPartitionMaker>(3);
   cid = new ContractorVarCID(ctc, x, std::move(slicer));
}

void clean()
{
   delete cid;
   delete B;
   delete P;
}

void test_1()
{
   B->set(x, Interval(-2, 4));
   B->set(y, Interval(1, 2));

   Proof p = cid->contract(*B);

   TEST_TRUE(p == Proof::Maybe);
   TEST_TRUE(B->get(x).isSetEq(Interval(1, 2)));
   TEST_TRUE(B->get(y).isSetEq(Interval(1, 2)));
}

void test_2()
{
   B->set(x, Interval(-2, 0));
   B->set(y, Interval(1, 2));

   Proof p = cid->contract(*B);

   TEST_TRUE(p == Proof::Empty);
}

void test_3()
{
   B->set(x, Interval(-2, 4));
   B->set(y, Interval(-3, 2));

   Proof p = cid->contract(*B);

   TEST_TRUE(p == Proof::Maybe);
   TEST_TRUE(B->get(x).isSetEq(Interval(-2, 2)));
   TEST_TRUE(B->get(y).isSetEq(Interval(-2, 2)));
}

int main()
{
   INIT_TEST

   TEST(test_1)
   TEST(test_2)
   TEST(test_3)

   CLEAN_TEST
   END_TEST
}
