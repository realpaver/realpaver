#include "test_config.hpp"
#include "realpaver/FlatFunction.hpp"
#include "realpaver/Problem.hpp"

Problem* P;
Variable x, y, z;

void init()
{
   P = new Problem();
   x = P->addRealVar(-10, 10, "x");
   y = P->addRealVar(-10, 10, "y");
   z = P->addRealVar(-10, 10, "z");

}

void clean()
{
   delete P;
}

void test_1()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(-1.5, 4));
   B.set(y, Interval(-1, 10.5));
   B.set(z, Interval(0, 2.5));
   Term t(sqr(x) - 2*y + log(z));
   FlatFunction f(t, Interval::zero());
   Interval I = t.eval(B),
            J = f.iEval(B);
   TEST_TRUE(I.isSetEq(J));
}

void test_2()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(-0.5, 27.25));
   B.set(y, Interval(-1, 10.5));
   B.set(z, Interval(0, 2.5));
   Term t(x*y - pow(z, 3));
   FlatFunction f(t, Interval::zero());
   Interval I = t.eval(B),
            J = f.iEval(B);
   TEST_TRUE(I.isSetEq(J));
}

void test_3()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(-1.5, 4));
   B.set(y, Interval(-1, 10.5));
   B.set(z, Interval(-100, 2.5));
   Term t(sqr(cos(x - z)) + sqr(cos(y)) - MIN(x, z));
   FlatFunction f(t, Interval::zero());
   Interval I = t.eval(B),
            J = f.iEval(B);
   TEST_TRUE(I.isSetEq(J));
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
