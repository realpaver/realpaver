#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/Preprocessor.hpp"
#include "test_config.hpp"

Problem *pb1;
Variable u1, v1, w1;

void init()
{
   pb1 = new Problem();
   u1 = pb1->addRealVar(-10, 10, "u1");
   v1 = pb1->addRealVar(-10, 10, "v1");
   w1 = pb1->addRealVar(-10, 10, "w1");

   pb1->addCtr(u1 + 1 == 0);
   pb1->addCtr(u1 + v1 == 0);
   pb1->addCtr(2 * u1 - v1 + sqr(w1) - w1 == 1);
}

void clean()
{
   delete pb1;
}

void test_1()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*pb1, dest);

   TEST_TRUE(proc.isFixed(u1));
   TEST_TRUE(proc.isFixed(v1));
   TEST_FALSE(proc.isFixed(w1));
}

void test_2()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*pb1, dest);

   TEST_TRUE(proc.getFixedDomain(u1).isSetEq(Interval(-1.0)));
   TEST_TRUE(proc.getFixedDomain(v1).isSetEq(Interval(1.0)));
}

void test_3()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*pb1, dest);

   Scope scop(proc.fixedScope());

   TEST_TRUE(scop.contains(u1));
   TEST_TRUE(scop.contains(v1));
}

void test_4()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*pb1, dest);

   TEST_TRUE(!proc.allVarsFixed());
}

void test_5()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*pb1, dest);

   TEST_TRUE(proc.nbInactiveCtrs() == 2);
}

void test_6()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*pb1, dest);

   DomainBox box = proc.fixedRegion();

   TEST_TRUE(box.scope().size() == 2);
   TEST_TRUE(box.scope().contains(u1));
   TEST_TRUE(box.scope().contains(v1));
   TEST_FALSE(box.scope().contains(w1));
}

void test_7()
{
   Preprocessor proc;
   Problem dest;

   proc.apply(*pb1, dest);

   DomainBox box = proc.destRegion();
   Variable w = proc.srcToDestVar(w1);

   TEST_TRUE(box.scope().size() == 1);
   TEST_TRUE(box.scope().contains(w));
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
