#include "test_config.hpp"
#include "realpaver/Bitset.hpp"

 int test_init_1()
{
   size_t n = 5;
   Bitset b(n);
   return b.size() == n && b.nbZeros() == n;
}

int test_init_2()
{
   size_t n = 5;
   Bitset b(n);
   b.setAllOne();
    return b.size() == n && b.nbOnes() == n;
}

int test_set()
{
   size_t n = 125, i = 97;
   Bitset b(n);
   b.setOne(i);
   return b.get(i) != 0 && b.nbOnes() == 1;
}

int test_flip_1()
{
   size_t n = 302, i = 146;
   Bitset b(n);
   b.setOne(i);
   if (b.get(i) == 0) return 0;
   b.flip(i);
   return b.get(i) == 0;
}

int test_flip_2()
{
   size_t n = 128, i = 64;
   Bitset b(n);
   b.setOne(i);
   b.flipAll();
   return b.get(i) == 0 && b.nbOnes() == n-1;
}

int test_and()
{
   size_t n = 45, i = 7, j = 12, k = 22, l = 36;
   Bitset b1(n), b2(n);
   b1.setOne(i); b1.setOne(j);
   b2.setOne(i); b2.setOne(k); b2.setOne(l);

   Bitset b3( b1 & b2 );
      
   return b3.get(i) != 0 && b3.get(j) == 0 &&
          b3.get(k) == 0 && b3.get(l) == 0 &&
          b3.nbOnes() == 1;
}

int test_or()
{
   size_t n = 500, i = 0, j = 229, k = 341, l = 499;
   Bitset b1(n), b2(n);
   b1.setOne(i); b1.setOne(j);
   b2.setOne(i); b2.setOne(l);

   Bitset b3( b1 | b2 );
      
   return b3.get(i) != 0 && b3.get(j) != 0 &&
          b3.get(k) == 0 && b3.get(l) != 0 &&
          b3.nbOnes() == 3;
}

int test_overlap()
{
   size_t n = 220, i = 70, j = 102, k = 103, l = 199;
   Bitset b1(n), b2(n), b3(n);
   b1.setOne(i); b1.setOne(j);
   b2.setOne(j); b2.setOne(k);
   b3.setOne(k); b3.setOne(l);

   return b1.overlaps(b2) && b2.overlaps(b3) && !b1.overlaps(b3);
}

int test_not()
{
   size_t n = 28, i = 3, j = 20;
   Bitset b1(n);
   b1.setOne(i); b1.setOne(j);
   Bitset b2( ~b1 );

   return b2.get(i) == 0 && b2.get(j) == 0 &&
          b2.nbZeros() == 2 && b2.nbOnes() == n-2;
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

   exit(0);
}
