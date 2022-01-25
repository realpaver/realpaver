#include "realpaver/interval.hpp"
#include "realpaver_test.hpp"

class IntervalTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( IntervalTest );
   CPPUNIT_TEST( testAddInv1 );
   CPPUNIT_TEST( testAddInv2 );
   CPPUNIT_TEST( testAddInv3 );
   CPPUNIT_TEST( testAddInv4 );
   CPPUNIT_TEST( testAddInv5 );
   CPPUNIT_TEST( testMinInv1 );
   CPPUNIT_TEST( testMinInv2 );
   CPPUNIT_TEST( testMinInv3 );
   CPPUNIT_TEST( testMinInv4 );
   CPPUNIT_TEST( testMaxInv1 );
   CPPUNIT_TEST( testMaxInv2 );
   CPPUNIT_TEST( testMaxInv3 );
   CPPUNIT_TEST( testMaxInv4 );
   CPPUNIT_TEST( testSgnInv1 );
   CPPUNIT_TEST( testSgnInv2 );
   CPPUNIT_TEST( testSgnInv3 );
   CPPUNIT_TEST( testSgnInv4 );
   CPPUNIT_TEST( testSgnInv5 );
   CPPUNIT_TEST( testExtDiv1 );
   CPPUNIT_TEST( testExtDiv2 );
   CPPUNIT_TEST( testExtDiv3 );
   CPPUNIT_TEST( testExtDiv4 );
   CPPUNIT_TEST( testExtDiv5 );
   CPPUNIT_TEST( testExtDiv6 );
   CPPUNIT_TEST( testSetMinus1 );
   CPPUNIT_TEST( testSetMinus2 );
   CPPUNIT_TEST( testSetMinus3 );
   CPPUNIT_TEST( testSetMinus4 );
   CPPUNIT_TEST( testSetMinus5 );
   CPPUNIT_TEST_SUITE_END();

public:
   void setUp()
   {}

   void tearDown()
   {}

   void testAddInv1()
   {
      Interval x(-2,3), y(-3,4), z(0,8);
      TEST_TRUE( addPX(x,y,z).isSetEq(Interval(-2,3)) );
      TEST_TRUE( addPY(x,y,z).isSetEq(Interval(-3,4)) );
      TEST_TRUE( addPZ(x,y,z).isSetEq(Interval(0,7)) );
   }

   void testAddInv2()
   {
      Interval x(Interval::moreThan(-2)), y(-3,4),
               z(Interval::lessThan(8));
      TEST_TRUE( addPX(x,y,z).isSetEq(Interval(-2,11)) );
      TEST_TRUE( addPY(x,y,z).isSetEq(Interval(-3,4)) );
      TEST_TRUE( addPZ(x,y,z).isSetEq(Interval(-5,8)) );
   }

   void testAddInv3()
   {
      Interval x(Interval::moreThan(-2)), y(Interval::lessThan(0)),
               z(Interval::universe());
      TEST_TRUE( addPZ(x,y,z).isUniverse() );
   }

   void testAddInv4()
   {
      Interval x(0,1), y(-1,2), z(4,7);
      TEST_TRUE( addPZ(x,y,z).isEmpty() );
   }

   void testAddInv5()
   {
      Interval x(Interval::emptyset()), y(-1,2), z(4,7);
      TEST_TRUE( addPZ(x,y,z).isEmpty() );
   }

   void testMinInv1()
   {
      Interval x(0,5), y(1,4), z(2,6);
      TEST_TRUE( minPX(x,y,z).isSetEq(Interval(2,5)) );
      TEST_TRUE( minPY(x,y,z).isSetEq(Interval(2,4)) );
      TEST_TRUE( minPZ(x,y,z).isSetEq(Interval(2,4)) );
   }

   void testMinInv2()
   {
      Interval x(0,3), y(4,6), z(4,6);
      TEST_TRUE( minPZ(x,y,z).isEmpty() );
   }

   void testMinInv3()
   {
      Interval x(0,3), y(5,6), z(-1,7);
      TEST_TRUE( minPX(x,y,z).isSetEq(x) );
      TEST_TRUE( minPY(x,y,z).isSetEq(y) );
      TEST_TRUE( minPZ(x,y,z).isSetEq(x) );
   }

   void testMinInv4()
   {
      Interval x(Interval::universe()), y(Interval::lessThan(2)),
               z(Interval::universe());
      TEST_TRUE( minPX(x,y,z).isSetEq(x) );
      TEST_TRUE( minPY(x,y,z).isSetEq(y) );
      TEST_TRUE( minPZ(x,y,z).isSetEq(y) );
   }

   void testMaxInv1()
   {
      Interval x(0,7), y(1,4), z(2,6);
      TEST_TRUE( maxPX(x,y,z).isSetEq(Interval(0,6)) );
      TEST_TRUE( maxPY(x,y,z).isSetEq(Interval(1,4)) );
      TEST_TRUE( maxPZ(x,y,z).isSetEq(Interval(2,6)) );
   }

   void testMaxInv2()
   {
      Interval x(0,3), y(1,4), z(-2,2);
      TEST_TRUE( maxPX(x,y,z).isSetEq(Interval(0,2)) );
      TEST_TRUE( maxPY(x,y,z).isSetEq(Interval(1,2)) );
      TEST_TRUE( maxPZ(x,y,z).isSetEq(Interval(1,2)) );
   }

   void testMaxInv3()
   {
      Interval x(0,3), y(1,4), z(5,6);
      TEST_TRUE( maxPZ(x,y,z).isEmpty() );
   }

   void testMaxInv4()
   {
      Interval x(Interval::lessThan(3)), y(Interval::moreThan(2)),
               z(1,8);
      TEST_TRUE( maxPX(x,y,z).isSetEq(x) );
      TEST_TRUE( maxPY(x,y,z).isSetEq(Interval(2,8)) );
      TEST_TRUE( maxPZ(x,y,z).isSetEq(Interval(2,8)) );
   }

   void testSgnInv1()
   {
      Interval x(1,7), y(-1,1);
      TEST_TRUE( sgnPX(x,y).isSetEq(x) );
      TEST_TRUE( sgnPY(x,y).isSetEq(Interval(1,1)) );
   }

   void testSgnInv2()
   {
      Interval x(0,7), y(-1,1);
      TEST_TRUE( sgnPX(x,y).isSetEq(x) );
      TEST_TRUE( sgnPY(x,y).isSetEq(Interval(0,1)) );
   }

   void testSgnInv3()
   {
      Interval x(-1,7), y(0,1);
      TEST_TRUE( sgnPX(x,y).isSetEq(Interval(0,7)) );
      TEST_TRUE( sgnPY(x,y).isSetEq(Interval(0,1)) );
   }

   void testSgnInv4()
   {
      Interval x(-1,0), y(1);
      TEST_TRUE( sgnPX(x,y).isEmpty() );
   }

   void testSgnInv5()
   {
      Interval x(0,2), y(-1);
      TEST_TRUE( sgnPX(x,y).isEmpty() );
   }

   void testExtDiv1()
   {
      Interval x(4,8), y(-1,2);
      std::pair<Interval,Interval> z = extDiv(x,y);
      TEST_TRUE( z.first.isSetEq(Interval::lessThan(-4)) );
      TEST_TRUE( z.second.isSetEq(Interval::moreThan(2)) );
   }

   void testExtDiv2()
   {
      Interval x(-8,-4), y(-1,2);
      std::pair<Interval,Interval> z = extDiv(x,y);
      TEST_TRUE( z.first.isSetEq(Interval::lessThan(-2)) );
      TEST_TRUE( z.second.isSetEq(Interval::moreThan(4)) );
   }

   void testExtDiv3()
   {
      Interval x(-8,2), y(-1,2);
      std::pair<Interval,Interval> z = extDiv(x,y);
      TEST_TRUE( z.first.isUniverse() );
      TEST_TRUE( z.second.isEmpty() );
   }

   void testExtDiv4()
   {
      Interval x(-8,2), y(0,2);
      std::pair<Interval,Interval> z = extDiv(x,y);
      TEST_TRUE( z.first.isUniverse() );
      TEST_TRUE( z.second.isEmpty() );
   }

   void testExtDiv5()
   {
      Interval x(-8,2), y(-4,-2);
      std::pair<Interval,Interval> z = extDiv(x,y);
      TEST_TRUE( z.first.isSetEq(x/y) );
      TEST_TRUE( z.second.isEmpty() );
   }

   void testExtDiv6()
   {
      Interval x(Interval::emptyset()), y(-4,-2);
      std::pair<Interval,Interval> z = extDiv(x,y);
      TEST_TRUE( z.first.isEmpty() );
      TEST_TRUE( z.second.isEmpty() );
   }

   void testSetMinus1()
   {
      Interval x(0,4), y(1,4);
      std::pair<Interval,Interval> z = setminus(x,y);
      TEST_TRUE( z.first.isSetEq(Interval(0,1)) );
      TEST_TRUE( z.second.isEmpty() );
   }

   void testSetMinus2()
   {
      Interval x(0,4), y(4,4);
      std::pair<Interval,Interval> z = setminus(x,y);
      TEST_TRUE( z.first.isSetEq(x) );
      TEST_TRUE( z.second.isEmpty() );
   }

   void testSetMinus3()
   {
      Interval x(0,4), y(-1,4);
      std::pair<Interval,Interval> z = setminus(x,y);
      TEST_TRUE( z.first.isEmpty() );
      TEST_TRUE( z.second.isEmpty() );
   }

   void testSetMinus4()
   {
      Interval x(0,4), y(1,3);
      std::pair<Interval,Interval> z = setminus(x,y);
      TEST_TRUE( z.first.isSetEq(Interval(0,1)) );
      TEST_TRUE( z.second.isSetEq(Interval(3,4)) );
   }

   void testSetMinus5()
   {
      Interval x(0,4), y(1,5);
      std::pair<Interval,Interval> z = setminus(x,y);
      TEST_TRUE( z.first.isSetEq(Interval(0,1)) );
      TEST_TRUE( z.second.isEmpty() );
   }
};

REGISTER_TEST(IntervalTest)
