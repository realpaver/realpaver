#include "test_config.hpp"
#include "realpaver/ContractorVar3B.hpp"
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/Problem.hpp"

#include <memory>

using namespace std;

using Result = std::pair<Proof, Interval>;

Result my_test(const Interval& I)
{
   Problem P;
   Variable x = P.addRealVar(0, 10, "x");
   Constraint c( in(x, I) );

   // the domain of x is sliced in 10 parts: [0, 1] [1, 2] ... [9, 10]
   // the constraint is: x in I

   Scope scop( {x} );
   IntervalBox B(scop);

   shared_ptr<Contractor> ctc = make_shared<ContractorConstraint>(c);
   unique_ptr<IntervalSlicer> slicer = make_unique<IntervalPartitionMaker>(10);

   ContractorVar3B ctc3b(ctc, x, std::move(slicer));
   Proof p = ctc3b.contract(B);

   return make_pair(p, B.get(x));
}

void test_1()
{
   Result res = my_test(Interval(0.5, 1));
   TEST_TRUE(res.first == Proof::Maybe);
   TEST_TRUE(res.second.isSetEq(Interval(0, 2)));
}

void test_2()
{
   Result res = my_test(Interval(1.5, 5.5));
   TEST_TRUE(res.first == Proof::Maybe);
   TEST_TRUE(res.second.isSetEq(Interval(1, 6)));
}

void test_3()
{
   Result res = my_test(Interval(1.5, 1.75));
   TEST_TRUE(res.first == Proof::Maybe);
   TEST_TRUE(res.second.isSetEq(Interval(1, 2)));
}

void test_4()
{
   Result res = my_test(Interval(-1, 3.5));
   TEST_TRUE(res.first == Proof::Maybe);
   TEST_TRUE(res.second.isSetEq(Interval(0, 4)));
}

void test_5()
{
   Result res = my_test(Interval(3.5, 11));
   TEST_TRUE(res.first == Proof::Maybe);
   TEST_TRUE(res.second.isSetEq(Interval(3, 10)));
}

void test_6()
{
   Result res = my_test(Interval(11, 13));
   TEST_TRUE(res.first == Proof::Empty);
}

int main()
{
   TEST(test_1)
   TEST(test_2)
   TEST(test_3)
   TEST(test_4)
   TEST(test_5)
   TEST(test_6)

   END_TEST
}
