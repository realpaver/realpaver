#include "realpaver/Preprocessor.hpp"
#include "test_config.hpp"

Problem *src;
DomainBox *box;
Variable x, y, z;

void init()
{
   src = new Problem();
   x = src->addRealVar(-10, 10, "x");
   y = src->addRealVar(-10, 10, "y");
   z = src->addRealVar(-10, 10, "z");

   src->addCtr(x + 1 == 0);
   src->addCtr(x + y == 0);
   src->addCtr(2 * x - y + sqr(z) - z == 1);

   box = new DomainBox(src->scope());
}

void clean()
{
   delete box;
   delete src;
}

void test_1()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*src, dest);

   TEST_TRUE(proc.hasFixedDomain(x));
   TEST_TRUE(proc.hasFixedDomain(y));
   TEST_FALSE(proc.hasFixedDomain(z));
}

void test_2()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*src, dest);

   TEST_TRUE(proc.getFixedDomain(x).isSetEq(Interval(-1.0)));
   TEST_TRUE(proc.getFixedDomain(y).isSetEq(Interval(1.0)));
}

void test_3()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*src, dest);

   Scope scop(proc.fixedScope());

   TEST_TRUE(scop.contains(x));
   TEST_TRUE(scop.contains(y));
}

void test_4()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*src, dest);

   TEST_TRUE(!proc.allVarsFixed());
}

void test_5()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*src, dest);

   TEST_TRUE(proc.nbInactiveCtrs() == 2);
}

void test_6()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*src, dest);

   DomainBox box = proc.fixedRegion();

   TEST_TRUE(box.scope().size() == 2);
   TEST_TRUE(box.scope().contains(x));
   TEST_TRUE(box.scope().contains(y));
   TEST_FALSE(box.scope().contains(z));
}

void test_7()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*src, dest);

   DomainBox box = proc.destRegion();
   Variable v = proc.srcToDestVar(z);

   TEST_TRUE(box.scope().size() == 1);
   TEST_TRUE(box.scope().contains(v));
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
