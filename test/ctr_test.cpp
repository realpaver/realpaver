#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"

Problem *prob;
Variable x, y, z;

void init()
{
   prob = new Problem();
   x = prob->addRealVar(0, 0, "x");
   y = prob->addRealVar(0, 0, "y");
   z = prob->addRealVar(0, 0, "z");

   prob->addCtr(2 * x - y + z == 1);
   prob->addCtr(sqr(x) + y * z - 2 * z + 2 == 0);
   prob->addCtr(sqr(x + y) - 2 * pow(z, 4) + 2 >= 0);
   prob->addCtr(cos(x + y) - 2 * pow(z, 4) + 2 >= 0);
}

void clean()
{
   delete prob;
}

void test_1()
{
   Constraint c0 = prob->ctrAt(0);
   TEST_TRUE(c0.isLinear());

   Constraint c1 = prob->ctrAt(1);
   TEST_TRUE(!c1.isLinear() && c1.isQuadratic());

   Constraint c2 = prob->ctrAt(2);
   TEST_TRUE(!c2.isLinear() && !c2.isQuadratic() && c2.isPolynomial());

   Constraint c3 = prob->ctrAt(3);
   TEST_TRUE(!c3.isLinear() && !c3.isQuadratic() && !c3.isPolynomial());
}

int main()
{
   INIT_TEST

   TEST(test_1)

   CLEAN_TEST
   END_TEST
}
