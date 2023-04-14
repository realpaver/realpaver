#include "test_config.hpp"
#include "realpaver/Scope.hpp"

Variable v("v"), w("w"), x("x"), y("y");

void test_init()
{
    Scope s;
    TEST_TRUE(s.isEmpty())
}

void test_insert()
{
   Scope s;
   s.insert(v);
   TEST_TRUE(s.size() == 1)

   s.insert(v);
   TEST_TRUE(s.size() == 1)

   s.insert(w);
   TEST_TRUE(s.size() == 2)
}

void test_member()
{
   Scope s;
   s.insert(x);
   TEST_TRUE(s.contains(x))
   TEST_FALSE(s.contains(y))
}

void test_counter()
{
   Scope s;
   s.insert(v);
   s.insert(v);
   TEST_TRUE(s.count(w) == 0)
   TEST_TRUE(s.count(v) == 2)
}

void test_remove()
{
   Scope s;
   s.insert(v);
   TEST_TRUE(s.contains(v))
   s.remove(v);
   TEST_FALSE(s.contains(v))
}

void test_overlap()
{
   Scope s1, s2;
   s1.insert(v);
   s1.insert(w);
   s2.insert(v);
   TEST_TRUE(s1.overlaps(s2))
   TEST_TRUE(s1.contains(s2))
   TEST_FALSE(s2.contains(s1))
}

int main()
{
   TEST(test_init)
   TEST(test_insert)
   TEST(test_member)
   TEST(test_counter)
   TEST(test_remove)
   TEST(test_overlap)

   END_TEST
}
