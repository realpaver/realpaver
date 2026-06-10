#include "realpaver/IntervalNewton.hpp"
#include "realpaver/IntervalNewtonParametric.hpp"
#include "test_config.hpp"

void test_parametric_1()
{
   Problem pbm;
   Variable w = pbm.addRealVar(-1.0, 1.0, "w"), x = pbm.addRealVar(2.5, 3.25, "x"),
            y = pbm.addRealVar(-2, -0.5, "y"), z = pbm.addRealVar(0, 2, "z");

   IntervalFunctionVector F({x - y + z - 1, x + y + 2 * z + 2});
   IntervalNewtonParametric N(F);

   IntervalBox B(pbm.scope()), mid(B.midpoint());
   Proof proof = N.certify(mid);

   TEST_TRUE(proof == Proof::Feasible);

   for (size_t i = 0; i < F.nbFuns(); ++i)
   {
      Interval e = F.fun(i).eval(mid);
      TEST_TRUE(e.containsZero() && e.width() <= 1.0e-12);
   }
}

void test_parametric_2()
{
   Problem pbm;
   Variable w = pbm.addRealVar(-1.0, 1.0, "w"), x = pbm.addRealVar(0.5, 2.25, "x"),
            y = pbm.addRealVar(-1, 2, "y"), z = pbm.addRealVar(-1, 0.5, "z");

   IntervalFunctionVector F({sqr(x) - y + z, x - sqr(y) * z - 1});
   IntervalNewtonParametric N(F);

   IntervalBox B(pbm.scope()), mid(B.midpoint());
   Proof proof = N.certify(mid);

   TEST_TRUE(proof == Proof::Feasible);

   for (size_t i = 0; i < F.nbFuns(); ++i)
   {
      Interval e = F.fun(i).eval(mid);
      TEST_TRUE(e.containsZero() && e.width() <= 1.0e-12);
   }
}

void test_parametric_3()
{
   Problem pbm;
   Variable w = pbm.addRealVar(-1.0, 1.0, "w"), x = pbm.addRealVar(0.5, 2.25, "x"),
            y = pbm.addRealVar(-1, 2, "y"), z = pbm.addRealVar(-1, 0.5, "z");

   IntervalFunctionVector F({sqr(x) - y + z});
   IntervalNewtonParametric N(F);

   IntervalBox B(pbm.scope()), mid(B.midpoint());
   Proof proof = N.certify(mid);

   TEST_TRUE(proof == Proof::Feasible);

   for (size_t i = 0; i < F.nbFuns(); ++i)
   {
      Interval e = F.fun(i).eval(mid);
      TEST_TRUE(e.containsZero() && e.width() <= 1.0e-12);
   }
}

int main()
{
   TEST(test_parametric_1)
   TEST(test_parametric_2)
   TEST(test_parametric_3)

   END_TEST
}
