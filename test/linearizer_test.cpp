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

void test_1()
{
}

int main()
{
   INIT_TEST

   TEST(test_1)

   CLEAN_TEST
   END_TEST
}
