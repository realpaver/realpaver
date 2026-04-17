#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/Preprocessor.hpp"
#include "test_config.hpp"

SharedProblem pb1;
Variable u1, v1, w1;

void init()
{
   pb1 = std::make_shared<Problem>();
   u1 = pb1->addRealVar(-10, 10, "u1");
   v1 = pb1->addRealVar(-10, 10, "v1");
   w1 = pb1->addRealVar(-10, 10, "w1");

   pb1->addCtr(u1 + 1 == 0);
   pb1->addCtr(u1 + v1 == 0);
   pb1->addCtr(2 * u1 - v1 + sqr(w1) - w1 == 1);
}

void clean()
{
}

void test_1()
{
   std::cout << "PRE 10" << std::endl;

   Preprocessor pp(pb1);

   std::cout << "PRE 20" << std::endl;

   pp.apply();

   std::cout << "PRE 30" << std::endl;

   TEST_TRUE(pp.isFixed(u1));
   TEST_TRUE(pp.isFixed(v1));
   TEST_FALSE(pp.isFixed(w1));

   std::cout << "PRE 100" << std::endl;
}

void test_2()
{
   Preprocessor pp(pb1);
   pp.apply();
   TEST_TRUE(pp.getFixedDomain(u1).isSetEq(Interval(-1.0)));
   TEST_TRUE(pp.getFixedDomain(v1).isSetEq(Interval(1.0)));
}

void test_3()
{
   Preprocessor pp(pb1);
   pp.apply();
   Scope scop(pp.fixedScope());
   TEST_TRUE(scop.contains(u1));
   TEST_TRUE(scop.contains(v1));
}

void test_4()
{
   Preprocessor pp(pb1);
   pp.apply();
   TEST_TRUE(!pp.allVarsFixed());
}

void test_5()
{
   Preprocessor pp(pb1);
   pp.apply();
   TEST_TRUE(pp.nbInactiveCtrs() == 2);
}

void test_6()
{
   Preprocessor pp(pb1);
   pp.apply();
   DomainBox box = pp.fixedRegion();
   TEST_TRUE(box.scope().size() == 2);
   TEST_TRUE(box.scope().contains(u1));
   TEST_TRUE(box.scope().contains(v1));
   TEST_FALSE(box.scope().contains(w1));
}

void test_7()
{
   Preprocessor pp(pb1);
   pp.apply();
   DomainBox box = pp.destRegion();
   Variable w = pp.srcToDestVar(w1);
   TEST_TRUE(box.scope().size() == 1);
   TEST_TRUE(box.scope().contains(w));
}

int main()
{
   INIT_TEST

   TEST(test_1)
   // TEST(test_2)
   // TEST(test_3)
   // TEST(test_4)
   // TEST(test_5)
   // TEST(test_6)
   // TEST(test_7)

   CLEAN_TEST
   END_TEST
}
