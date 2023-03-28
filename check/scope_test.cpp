#include "realpaver/Scope.hpp"
#include "realpaver_test.hpp"

class ScopeTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( ScopeTest );
   CPPUNIT_TEST( testInit );
   CPPUNIT_TEST( testInsertion );
   CPPUNIT_TEST( testMembership );
   CPPUNIT_TEST( testCounter );
   CPPUNIT_TEST( testRemoval );
   CPPUNIT_TEST( testInclusion );
   CPPUNIT_TEST_SUITE_END();

   Variable v, w, x, y;

public:
   void setUp()
   {
      v = Variable("v"); v.setId(0);
      w = Variable("w"); w.setId(1);
      x = Variable("x"); x.setId(2);
      y = Variable("y"); y.setId(3);
   }

   void tearDown()
   {}

   void testInit()
   {
      Scope s;
      TEST_TRUE( s.isEmpty() );
   }

   void testInsertion()
   {
      Scope s;

      s.insert(v);
      TEST_TRUE( s.size() == 1 );

      s.insert(v);
      TEST_TRUE( s.size() == 1 );

      s.insert(w);
      TEST_TRUE( s.size() == 2 );
   }

   void testMembership()
   {
      Scope s;

      s.insert(x);
      TEST_TRUE( s.contains(x) );
      TEST_FAIL( s.contains(y) );
   }

   void testCounter()
   {
      Scope s;

      s.insert(v);
      s.insert(v);

      TEST_TRUE( s.count(w) == 0 );
      TEST_TRUE( s.count(v) == 2 );
   }

   void testRemoval()
   {
      Scope s;

      s.insert(v);
      TEST_TRUE( s.contains(v) );

      s.remove(v);
      TEST_FAIL( s.contains(v) );
   }

   void testInclusion()
   {
      Scope s1, s2;

      s1.insert(v);
      s1.insert(w);

      s2.insert(v);
      TEST_TRUE( s1.overlaps(s2) );
      TEST_TRUE( s1.contains(s2) );
      TEST_FAIL( s2.contains(s1) );      
   }
};

REGISTER_TEST(ScopeTest)
