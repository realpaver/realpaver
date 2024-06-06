#include "test_config.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"

Problem* prob;
Dag* dag;
IntervalBox* B;
Variable x, y, z;

void init()
{
   prob = new Problem();
   x = prob->addRealVar(0, 0, "x");
   y = prob->addRealVar(0, 0, "y");
   z = prob->addRealVar(0, 0, "z");

   dag = new Dag();
   dag->insert(sqr(x + y) - 2*z + 2 == 0);
   dag->insert(sqr(x + y) - 2*z + 2 >= 0);
   dag->insert(2*x - y + z == 1);

   B = new IntervalBox(prob->scope());
}

void clean()
{
   delete B;
   delete dag;
   delete prob;
}

void test_1()
{
   B->set(x, Interval(-10, 15));
   B->set(y, Interval(-20, 5));
   B->set(z, Interval(-10, 5.5));

   Proof p = dag->fun(0)->hc4Revise(*B);

   TEST_TRUE(p == Proof::Maybe);
   TEST_TRUE(B->get(x).isSetEq(Interval(-8, 15)));
   TEST_TRUE(B->get(y).isSetEq(Interval(-18, 5)));
   TEST_TRUE(B->get(z).isSetEq(Interval(1, 5.5)));
}

void test_2()
{
   B->set(x, Interval::universe());
   B->set(y, Interval(-20, 5));
   B->set(z, Interval(-10, 5.5));

   Proof p = dag->fun(0)->hc4Revise(*B);

   TEST_TRUE(p == Proof::Maybe);
   TEST_TRUE(B->get(x).isSetEq(Interval(-8, 23)));
   TEST_TRUE(B->get(y).isSetEq(Interval(-20, 5)));
   TEST_TRUE(B->get(z).isSetEq(Interval(1, 5.5)));
}

void test_3()
{
   B->set(x, Interval(-10, 15));
   B->set(y, Interval(-20, 5));
   B->set(z, Interval(-10, 0));

   Proof p = dag->fun(0)->hc4Revise(*B);

   TEST_TRUE(p == Proof::Empty);
}

void test_4()
{
   B->set(x, Interval(2, 4));
   B->set(y, Interval(3, 10));
   B->set(z, Interval(0, 6));

   Proof p = dag->fun(1)->hc4Revise(*B);

   TEST_TRUE(p == Proof::Inner);
}

void test_5()
{
   B->set(x, Interval(1, 8));
   B->set(y, Interval(-10, 10));
   B->set(z, Interval(-1, 4));

   Proof p = dag->fun(2)->hc4Revise(*B);

   TEST_TRUE(p == Proof::Maybe);
   TEST_TRUE(B->get(x).isSetEq(Interval(1, 6)));
   TEST_TRUE(B->get(y).isSetEq(Interval(0, 10)));
   TEST_TRUE(B->get(z).isSetEq(Interval(-1, 4)));
}

int main()
{
   INIT_TEST

   TEST(test_1)
   TEST(test_2)
   TEST(test_3)
   TEST(test_4)
   TEST(test_5)

   CLEAN_TEST
   END_TEST
}
