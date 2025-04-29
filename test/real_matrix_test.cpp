#include "realpaver/RealMatrix.hpp"
#include "test_config.hpp"

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

void test_LU_1()
{
   RealMatrix M({{4, 3}, {6, 3}});
   RealMatrix L(2, 2), U(2, 2);

   M.LU(L, U);
   TEST_TRUE(M == L * U);
}

void test_LU_2()
{
   RealMatrix M({{2, -1, 2}, {-4, 6, 3}, {-4, -2, 8}});
   RealMatrix L(3, 3), U(3, 3);

   M.LU(L, U);
   TEST_TRUE(M == L * U);
}

void test_LU_3()
{
   RealMatrix M({{2, -1, -2}, {-4, 6, 3}, {-4, -2, 8}});
   RealMatrix L(3, 3), U(3, 3);

   M.LU(L, U);
   TEST_TRUE(M == L * U);
}

void test_LU_4()
{
   RealMatrix M({{25, 5, 4}, {10, 8, 16}, {8, 12, 22}});
   RealMatrix L(3, 3), U(3, 3);

   M.LU(L, U);
   TEST_TRUE(M == L * U);
}

int main()
{
   TEST(test_inv_1)
   TEST(test_inv_2)
   TEST(test_inv_3)
   TEST(test_inv_4)
   TEST(test_LU_1)
   TEST(test_LU_2)
   TEST(test_LU_3)
   TEST(test_LU_4)

   END_TEST
}
