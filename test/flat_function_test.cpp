#include "realpaver/FlatFunction.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"

Problem *P;
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

void test_eval_1()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(-1.5, 4));
   B.set(y, Interval(-1, 10.5));
   B.set(z, Interval(0, 2.5));
   Term t(sqr(x) - 2 * y + log(z));
   FlatFunction f(t, Interval::zero());
   Interval I = t.eval(B), J = f.iEval(B);
   TEST_TRUE(I.isSetEq(J));
}

void test_eval_2()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(-0.5, 27.25));
   B.set(y, Interval(-1, 10.5));
   B.set(z, Interval(0, 2.5));
   Term t(x * y - pow(z, 3));
   FlatFunction f(t, Interval::zero());
   Interval I = t.eval(B), J = f.iEval(B);
   TEST_TRUE(I.isSetEq(J));
}

void test_eval_3()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(-1.5, 4));
   B.set(y, Interval(-1, 10.5));
   B.set(z, Interval(-100, 2.5));
   Term t(sqr(cos(x - z)) + sqr(cos(y)) - MIN(x, z));
   FlatFunction f(t, Interval::zero());
   Interval I = t.eval(B), J = f.iEval(B);
   TEST_TRUE(I.isSetEq(J));
}

void test_diff_1()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(-1.5, 4));
   B.set(y, Interval(-1, 2.5));

   Term t = x * sqr(y) - y * pow(x, 3) - 2 * x + y - 2;

   FlatFunction f(t, Interval::zero());
   IntervalVector G(2);
   f.iDiff(B, G);

   TEST_TRUE(G[0].isSetEq(Interval(-122, 52.25)));
   TEST_TRUE(G[1].isSetEq(Interval(-71, 24.375)));
}

void test_diff_2()
{
   IntervalBox B(P->scope());
   B.set(x, Interval(1, 4));
   B.set(y, Interval(1, 2));

   Term t = x / y;

   FlatFunction f(t, Interval::zero());
   IntervalVector G(2);
   f.iDiff(B, G);

   TEST_TRUE(G[0].isSetEq(Interval(0.5, 1)));
   TEST_TRUE(G[1].isSetEq(Interval(-4, -0.25)));
}

int main()
{
   INIT_TEST

   TEST(test_eval_1)
   TEST(test_eval_2)
   TEST(test_eval_3)

   TEST(test_diff_1)
   TEST(test_diff_2)

   CLEAN_TEST
   END_TEST
}
