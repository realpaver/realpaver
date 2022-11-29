#include "Integer.hpp"
#include "realpaver_test.hpp"

class IntegerTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( IntegerTest );
   CPPUNIT_TEST( testInit );
   CPPUNIT_TEST( testOverflowAdd );
   CPPUNIT_TEST( testOverflowSub );
   CPPUNIT_TEST( testOverflowMul );
   CPPUNIT_TEST( testAdd );
   CPPUNIT_TEST( testSub );
   CPPUNIT_TEST( testUsub );
   CPPUNIT_TEST( testMul );
   CPPUNIT_TEST( testDiv );
   CPPUNIT_TEST( testMod );
   CPPUNIT_TEST( testAbs );
   CPPUNIT_TEST( testSqr );
   CPPUNIT_TEST( testDivFloor );
   CPPUNIT_TEST( testDivCeil );
   CPPUNIT_TEST( testSqrtFloor );
   CPPUNIT_TEST( testSqrtCeil );
   CPPUNIT_TEST( testMin );
   CPPUNIT_TEST( testMax );
   CPPUNIT_TEST( testFloor );
   CPPUNIT_TEST( testCeil );
   CPPUNIT_TEST( testCastDouble );
   CPPUNIT_TEST_SUITE_END();

   Integer min_, max_, zero_;

public:
   void setUp()
   {
      min_ = Integer::MIN();
      max_ = Integer::MAX();
      zero_ = 0;
   }

   void tearDown()
   {}

   void testInit()
   {
      TEST_TRUE( Integer(0) == 0);
      TEST_TRUE( Integer(1) != 0);
      TEST_TRUE( Integer(1) > 0);
      TEST_TRUE( Integer(1) >= 0);
      TEST_TRUE( Integer(-1) <= 0);
      TEST_TRUE( Integer(-1) < 0);
      TEST_TRUE(min_ < 0);
      TEST_TRUE(max_ > 0);
   }

   void testOverflowAdd()
   {
      Integer x;
      TEST_THROW( x = max_ + 1 );
   }

   void testOverflowSub()
   {
      Integer x;
      TEST_THROW( x = min_ - 1 );
   }

   void testUsub()
   {
      Integer x = -7, y = 7;
      TEST_TRUE( (-x) == y );
      TEST_TRUE( (-y) == x );
   }

   void testOverflowMul()
   {
      Integer x;
      TEST_THROW( x = max_ * 2 );
      TEST_THROW( x = min_ * 2 );
   }

   void testAdd()
   {
      TEST_TRUE( min_ + zero_  == min_ );
      TEST_TRUE( max_ + zero_  == max_ );      
   }

   void testSub()
   {
      TEST_TRUE( min_ - zero_  == min_ );
      TEST_TRUE( max_ - zero_  == max_ );      
   }

   void testMul()
   {
      TEST_TRUE( min_ * zero_  == zero_ );
      TEST_TRUE( max_ * zero_  == zero_ );      
   }

   void testDiv()
   {
      Integer x;
      TEST_THROW( x = max_ / 0 );
      TEST_TRUE( min_ / min_  == 1 );
      TEST_TRUE( max_ / max_  == 1 );      
   }

   void testMod()
   {
      Integer x;
      TEST_THROW( x = max_ % 0 );

      if (min_.isEven())
         TEST_TRUE(min_ % 2 == 0);
      else
         TEST_TRUE(min_ % 2 != 0);

      if (max_.isEven())
         TEST_TRUE(max_ % 2 == 0);
      else
         TEST_TRUE(max_ % 2 != 0);
   }

   void testAbs()
   {
      Integer x = 1, y = -1;
      TEST_TRUE( abs(y) == x );
      TEST_TRUE( abs(x) == x );
   }

   void testSqr()
   {
      Integer x = 8, y = 64;
      TEST_TRUE( sqr(zero_) == 0 );
      TEST_TRUE( sqr(x) == y);
   }

   void testDivFloor()
   {
      Integer x = 13, y = 4, u = -13, v = -4;
      TEST_TRUE( divFloor(x,y) == 3);
      TEST_TRUE( divFloor(u,v) == 3);
      TEST_TRUE( divFloor(x,v) == -4);
      TEST_TRUE( divFloor(u,y) == -4);
      TEST_TRUE( divFloor(x,u) == -1);      
   }

   void testDivCeil()
   {
      Integer x = 13, y = 4, u = -13, v = -4;
      TEST_TRUE( divCeil(x,y) == 4);
      TEST_TRUE( divCeil(u,v) == 4);
      TEST_TRUE( divCeil(x,v) == -3);
      TEST_TRUE( divCeil(u,y) == -3);
      TEST_TRUE( divCeil(x,u) == -1);            
   }

   void testSqrtFloor()
   {
      Integer x = 13, y = 16;
      TEST_TRUE( sqrtFloor(x) == 3);
      TEST_TRUE( sqrtFloor(y) == 4);
   }

   void testSqrtCeil()
   {
      Integer x = 13, y = 16;
      TEST_TRUE( sqrtCeil(x) == 4);
      TEST_TRUE( sqrtCeil(y) == 4);      
   }

   void testMin()
   {
      Integer x = 3, y = 4, u = -3, v = -4;
      TEST_TRUE( min(x,y) == x );
      TEST_TRUE( min(x,u) == u );
      TEST_TRUE( min(u,v) == v );
   }

   void testMax()
   {
      Integer x = 3, y = 4, u = -3, v = -4;
      TEST_TRUE( max(x,y) == y );
      TEST_TRUE( max(x,u) == x );
      TEST_TRUE( max(u,v) == u );      
   }

   void testFloor()
   {
      double x = 1.5, y = -1.5, z = 2.0;
      TEST_TRUE( Integer::floor(x) == 1 );
      TEST_TRUE( Integer::floor(y) == -2 );
      TEST_TRUE( Integer::floor(z) == 2 );

      Integer si;
      double u;

      u = Integer::MAX() * 2.0;
      TEST_THROW( si = Integer::floor(u) );

      u = Integer::MIN() * 2.0;
      TEST_THROW( si = Integer::floor(u) );
   }

   void testCeil()
   {
      double x = 1.5, y = -1.5, z = 2.0;
      TEST_TRUE( Integer::ceil(x) == 2 );
      TEST_TRUE( Integer::ceil(y) == -1 );
      TEST_TRUE( Integer::ceil(z) == 2 );

      Integer si;
      double u;

      u = Integer::MAX() * 2.0;
      TEST_THROW( si = Integer::ceil(u) );

      u = Integer::MIN() * 2.0;
      TEST_THROW( si = Integer::ceil(u) );
   }

   void testCastDouble ()
   {
      TEST_TRUE( min_.toDouble() == min_.get() );
      TEST_TRUE( max_.toDouble() == max_.get() );
   }
};

REGISTER_TEST(IntegerTest)
