#include "test_config.hpp"
#include "realpaver/RealMatrix.hpp"

void test_inv_1()
{
   RealMatrix M({{1, -1}, {-2, 1}});
   RealMatrix Q({{-1, -1}, {-2, -1}});

   RealMatrix P(2, 2);
   bool b = M.inverse(P);

   TEST_TRUE(b);
   TEST_TRUE(P == Q);
}

void test_inv_2()
{
   RealMatrix M({{1, 1, 1}, {1, 2, 2}, {1, 2, 3}});
   RealMatrix Q({{2, -1, 0}, {-1, 2, -1}, {0, -1, 1}});

   RealMatrix P(3, 3);
   bool b = M.inverse(P);

   TEST_TRUE(b);
   TEST_TRUE(P == Q);
}

void test_inv_3()
{
   RealMatrix M({{1, -1}, {2, -2}});

   RealMatrix P(2, 2);
   bool b = M.inverse(P);

   TEST_FALSE(b);
}

void test_inv_4()
{
   RealMatrix M({{1, -1, 2}, {0, 1, 4}, {1, 2, 14}});

   RealMatrix P(3, 3);
   bool b = M.inverse(P);

   TEST_FALSE(b);
}


int main()
{
   TEST(test_inv_1)
   TEST(test_inv_2)
   TEST(test_inv_3)
   TEST(test_inv_4)

   END_TEST
}
