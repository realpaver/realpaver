#include "test_config.hpp"
#include "realpaver/Bitset.hpp"

void test_init_1()
{
   size_t n = 5;
   Bitset b(n);
   TEST_TRUE(b.size() == n);
   TEST_TRUE(b.nbZeros() == n);
}

void test_init_2()
{
   size_t n = 5;
   Bitset b(n);
   b.setAllOne();
   TEST_TRUE(b.size() == n);
   TEST_TRUE(b.nbOnes() == n);
}

void test_set()
{
   size_t n = 125, i = 97;
   Bitset b(n);
   b.setOne(i);
   TEST_TRUE(b.get(i) != 0);
   TEST_TRUE(b.nbOnes() == 1);
}

void test_flip_1()
{
   size_t n = 302, i = 146;
   Bitset b(n);
   b.setOne(i);
   TEST_FALSE(b.get(i) == 0);

   b.flip(i);
   TEST_TRUE(b.get(i) == 0);
}

void test_flip_2()
{
   size_t n = 128, i = 64;
   Bitset b(n);
   b.setOne(i);
   b.flipAll();
   TEST_TRUE(b.get(i) == 0);
   TEST_TRUE(b.nbOnes() == n-1);
}

void test_and()
{
   size_t n = 45, i = 7, j = 12, k = 22, l = 36;
   Bitset b1(n), b2(n);
   b1.setOne(i); b1.setOne(j);
   b2.setOne(i); b2.setOne(k); b2.setOne(l);

   Bitset b3(b1 & b2);
      
   TEST_TRUE(b3.get(i) != 0);
   TEST_TRUE(b3.get(j) == 0);
   TEST_TRUE(b3.get(k) == 0);
   TEST_TRUE(b3.get(l) == 0);
   TEST_TRUE(b3.nbOnes() == 1);
}

void test_or()
{
   size_t n = 500, i = 0, j = 229, k = 341, l = 499;
   Bitset b1(n), b2(n);
   b1.setOne(i); b1.setOne(j);
   b2.setOne(i); b2.setOne(l);

   Bitset b3(b1 | b2);
      
   TEST_TRUE(b3.get(i) != 0);
   TEST_TRUE(b3.get(j) != 0);
   TEST_TRUE(b3.get(k) == 0);
   TEST_TRUE(b3.get(l) != 0);
   TEST_TRUE(b3.nbOnes() == 3);
}

void test_overlap()
{
   size_t n = 220, i = 70, j = 102, k = 103, l = 199;
   Bitset b1(n), b2(n), b3(n);
   b1.setOne(i); b1.setOne(j);
   b2.setOne(j); b2.setOne(k);
   b3.setOne(k); b3.setOne(l);

   TEST_TRUE(b1.overlaps(b2));
   TEST_TRUE(b2.overlaps(b3));
   TEST_FALSE(b1.overlaps(b3));
}

void test_not()
{
   size_t n = 28, i = 3, j = 20;
   Bitset b1(n);
   b1.setOne(i); b1.setOne(j);
   Bitset b2(~b1);

   TEST_TRUE(b2.get(i) == 0);
   TEST_TRUE(b2.get(j) == 0);
   TEST_TRUE(b2.nbZeros() == 2);
   TEST_TRUE(b2.nbOnes() == n-2);
}

int main()
{
   TEST(test_init_1)
   TEST(test_init_2)
   TEST(test_set)
   TEST(test_flip_1)
   TEST(test_flip_2)
   TEST(test_and)
   TEST(test_or)
   TEST(test_overlap)
   TEST(test_not)

   END_TEST
}
