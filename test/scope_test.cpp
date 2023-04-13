#include "test_config.hpp"
#include "realpaver/Scope.hpp"

Variable v("v"), w("w"), x("x"), y("y");

int test_init()
 {
    Scope s;
    return s.isEmpty();
}

int test_insert()
{
   Scope s;
   s.insert(v);
   if (s.size() != 1) return 0;

   s.insert(v);
   if (s.size() != 1) return 0;

   s.insert(w);
   return s.size() == 2;
}

int test_member()
{
   Scope s;
   s.insert(x);
   return s.contains(x) && !s.contains(y);
}

int test_counter()
{
   Scope s;
   s.insert(v);
   s.insert(v);
   return s.count(w) == 0 && s.count(v) == 2;
}

int test_remove()
{
   Scope s;
   s.insert(v);
   if (!s.contains(v)) return 0;

   s.remove(v);
   return !s.contains(v);
}

int test_overlap()
{
   Scope s1, s2;
   s1.insert(v);
   s1.insert(w);
   s2.insert(v);
   return s1.overlaps(s2) && s1.contains(s2) && !s2.contains(s1);     
}

int main()
{
   TEST(test_init)
   TEST(test_insert)
   TEST(test_member)
   TEST(test_counter)
   TEST(test_remove)
   TEST(test_overlap)

   exit(0);
}
