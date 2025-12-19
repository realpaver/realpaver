#include "realpaver/Domain.hpp"
#include "realpaver/DomainPicker.hpp"
#include "test_config.hpp"

void test_1()
{
   Domain *dom = new IntervalDomain(Interval(0, 10));
   DomainPicker picker;
   dom->acceptVisitor(picker);
   TEST_TRUE(picker.getValue() == 5.0);
   delete dom;
}

void test_2()
{
   Domain *dom = new BinaryDomain();
   DomainPicker picker;
   dom->acceptVisitor(picker);
   TEST_TRUE(picker.getValue() == 0.0);
   delete dom;
}

void test_3()
{
   ZeroOne z(0, 1);
   Domain *dom = new BinaryDomain(z);
   DomainPicker picker;
   dom->acceptVisitor(picker);
   TEST_TRUE(picker.getValue() == 1.0);
   delete dom;
}

void test_4()
{
   Domain *dom = new RangeDomain(Range(0, 10));
   DomainPicker picker;
   dom->acceptVisitor(picker);
   TEST_TRUE(picker.getValue() == 0.0);
   delete dom;
}

void test_5()
{
   IntervalUnion U({Interval(0, 10), Interval(20, 30)});
   Domain *dom = new IntervalUnionDomain(U);
   DomainPicker picker;
   dom->acceptVisitor(picker);
   TEST_TRUE(picker.getValue() == 5.0);
   delete dom;
}

void test_6()
{
   RangeUnion U({Range(0, 10), Range(20, 30)});
   Domain *dom = new RangeUnionDomain(U);
   DomainPicker picker;
   dom->acceptVisitor(picker);
   TEST_TRUE(picker.getValue() == 0.0);
   delete dom;
}

int main()
{
   TEST(test_1)
   TEST(test_2)
   TEST(test_3)
   TEST(test_4)
   TEST(test_5)

   END_TEST
}
