#include "realpaver/Interval.hpp"
#include "realpaver/Range.hpp"
#include "realpaver_test.hpp"

class RangeTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( RangeTest );
   CPPUNIT_TEST( testInit );
   CPPUNIT_TEST( testPosEq );
   CPPUNIT_TEST( testCertEq );
   CPPUNIT_TEST( testPosNeq );
   CPPUNIT_TEST( testCertNeq );
   CPPUNIT_TEST( testPosLe );
   CPPUNIT_TEST( testCertLe );
   CPPUNIT_TEST( testPosLt );
   CPPUNIT_TEST( testCertLt );
   CPPUNIT_TEST( testPosGe );
   CPPUNIT_TEST( testCertGe );
   CPPUNIT_TEST( testPosGt );
   CPPUNIT_TEST( testCertGt );
   CPPUNIT_TEST( testInter );
   CPPUNIT_TEST( testHull );
   CPPUNIT_TEST( testRound );
   CPPUNIT_TEST( testContains );
   CPPUNIT_TEST( testStrictlyContains );
   CPPUNIT_TEST( testDisjoint );
   CPPUNIT_TEST( testAdd );
   CPPUNIT_TEST( testSub );
   CPPUNIT_TEST( testMulA );
   CPPUNIT_TEST( testMulB );
   CPPUNIT_TEST( testSqrA );
   CPPUNIT_TEST( testSqrB );
   CPPUNIT_TEST( testAbsA );
   CPPUNIT_TEST( testAbsB );
   CPPUNIT_TEST( testMin );
   CPPUNIT_TEST( testMax );
   CPPUNIT_TEST( testDivA );
   CPPUNIT_TEST( testDivB );
   CPPUNIT_TEST( testMod );
   CPPUNIT_TEST( testWidth );
   CPPUNIT_TEST_SUITE_END();

public:
   void setUp()
   {}

   void tearDown()
   {}

   void testInit()
   {
      Range e(Range::emptyset()), ee(3,0), u;
      TEST_TRUE(e.isEmpty());
      TEST_TRUE(ee.isEmpty());
      TEST_TRUE(u.isSetEq(Range::universe()));
   }

   void testPosEq()
   {
      Range x(0,7), y(-1,3), z(3,5),
            u(4,8), v(8,9);
      TEST_TRUE( x.isPossiblyEq(y) );
      TEST_TRUE( x.isPossiblyEq(z) );
      TEST_TRUE( x.isPossiblyEq(u) );
      TEST_FAIL( x.isPossiblyEq(v) );
      TEST_TRUE( y.isPossiblyEq(y) );
   }

   void testCertEq()
   {
      Range x(0,0), y(0,0), z(3,5);
      TEST_TRUE( x.isCertainlyEq(y) );
      TEST_FAIL( x.isCertainlyEq(z) );
   }

   void testPosNeq()
   {
      Range x(0,0), y(0,0), z(3,5);
      TEST_FAIL( x.isPossiblyNeq(y) );
      TEST_TRUE( x.isPossiblyNeq(z) );
   }

   void testCertNeq()
   {
      Range x(0,7), y(-1,3), z(8,9);
      TEST_FAIL( x.isCertainlyNeq(y) );
      TEST_TRUE( x.isCertainlyNeq(z) );
   }

   void testPosLe()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_TRUE( x.isPossiblyLe(y) );
      TEST_TRUE( x.isPossiblyLe(z) );
      TEST_TRUE( y.isPossiblyLe(x) );
      TEST_TRUE( z.isPossiblyLe(y) );
      TEST_FAIL( z.isPossiblyLe(x) );
   }

   void testCertLe()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_FAIL( x.isCertainlyLe(y) );
      TEST_TRUE( x.isCertainlyLe(z) );
      TEST_FAIL( y.isCertainlyLe(x) );
      TEST_FAIL( z.isCertainlyLe(y) );
   }

   void testPosLt()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_TRUE( x.isPossiblyLt(y) );
      TEST_TRUE( x.isPossiblyLt(z) );
      TEST_TRUE( y.isPossiblyLt(x) );
      TEST_FAIL( z.isPossiblyLt(y) );
      TEST_FAIL( z.isPossiblyLt(x) );
   }

   void testCertLt()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_FAIL( x.isCertainlyLt(y) );
      TEST_TRUE( x.isCertainlyLt(z) );
      TEST_FAIL( y.isCertainlyLt(z) );
      TEST_FAIL( z.isCertainlyLt(y) );
   }

   void testPosGe()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_TRUE( x.isPossiblyGe(y) );
      TEST_FAIL( x.isPossiblyGe(z) );
      TEST_TRUE( y.isPossiblyGe(x) );
      TEST_TRUE( z.isPossiblyGe(y) );
      TEST_TRUE( z.isPossiblyGe(x) );
   }

   void testCertGe()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_FAIL( x.isCertainlyGe(y) );
      TEST_FAIL( x.isCertainlyGe(z) );
      TEST_FAIL( y.isCertainlyGe(z) );
      TEST_TRUE( z.isCertainlyGe(y) );
      TEST_TRUE( z.isCertainlyGe(x) );
   }

   void testPosGt()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_TRUE( x.isPossiblyGt(y) );
      TEST_FAIL( x.isPossiblyGt(z) );
      TEST_TRUE( y.isPossiblyGt(x) );
      TEST_TRUE( z.isPossiblyGt(y) );
      TEST_TRUE( z.isPossiblyGt(x) );
   }

   void testCertGt()
   {
      Range x(0,4), y(2,8), z(8,9);
      TEST_FAIL( x.isCertainlyGt(y) );
      TEST_FAIL( x.isCertainlyGt(z) );
      TEST_FAIL( y.isCertainlyGt(x) );
      TEST_FAIL( z.isCertainlyGt(y) );
      TEST_TRUE( z.isCertainlyGt(x) );
   }

   void testInter()
   {
      Range x(0,4), y(2,8), z(8,9), e(Range::emptyset()),
            xy = x & y, xz = x & z, yz = y & z, xe = x & e, ee = e & e;
      TEST_TRUE( xy.isSetEq(Range(2,4)) );
      TEST_TRUE( xz.isEmpty() );
      TEST_TRUE( yz.isSingleton() );
      TEST_TRUE( xe.isEmpty() );
      TEST_TRUE( ee.isEmpty() );
   }

   void testHull()
   {
      Range x(0,4), y(2,8), z(8,9), e(Range::emptyset()),
            xy = x | y, xz = x | z, yz = y | z, xe = x | e, ee = e | e;
      TEST_TRUE( xy.isSetEq(Range(0,8)) );
      TEST_TRUE( xz.isSetEq(Range(0,9)) );
      TEST_TRUE( yz.isSetEq(Range(2,9)) );
      TEST_TRUE( xe.isSetEq(x) );
      TEST_TRUE( ee.isEmpty() );      
   }

   void testRound()
   {
      Interval x(-1.5,4.5), y(-2,10);
      TEST_TRUE( Range::roundInward(x).isSetEq(Range(-1,4)) );
      TEST_TRUE( Range::roundOutward(x).isSetEq(Range(-2,5)) );
      TEST_TRUE( Range::roundInward(y).isSetEq(Range(-2,10)) );
      TEST_TRUE( Range::roundOutward(y).isSetEq(Range(-2,10)) );
   }

   void testContains()
   {
      Range x(2,10), e(Range::emptyset());
      TEST_TRUE( x.contains(x) );
      TEST_TRUE( x.contains(Range(3,10)) );
      TEST_TRUE( x.contains(Range(2,9)) );
      TEST_TRUE( x.contains(Range(3,9)) );
      TEST_FAIL( x.contains(Range(1,9)) );
      TEST_TRUE( x.contains(e) );
      TEST_FAIL( e.contains(x) );
   }

   void testStrictlyContains()
   {
      Range x(2,10), e(Range::emptyset());
      TEST_FAIL( x.strictlyContains(x) );
      TEST_FAIL( x.strictlyContains(Range(3,10)) );
      TEST_FAIL( x.strictlyContains(Range(2,9)) );
      TEST_TRUE( x.strictlyContains(Range(3,9)) );
      TEST_FAIL( x.strictlyContains(Range(1,9)) );
      TEST_TRUE( x.strictlyContains(e) );
      TEST_FAIL( e.strictlyContains(x) );
   }

   void testDisjoint()
   {
      Range x(-3,4), y(0,5), z(6,9), e(Range::emptyset());
      TEST_TRUE( x.isDisjoint(z) );
      TEST_TRUE( x.isDisjoint(e) );
      TEST_TRUE( e.isDisjoint(x) );
      TEST_FAIL( x.isDisjoint(y) );
      TEST_TRUE( x.overlaps(y) );
      TEST_FAIL( x.overlaps(z) );
      TEST_FAIL( x.overlaps(e) );
   }

   void testAdd()
   {
      Range x(-2,5), y(1,8), z(-6,4), f(-1,13),
            px(-2,3), py(1,6), pz(-1,4);
      TEST_TRUE( f.isSetEq(x + y) );
      TEST_TRUE( px.isSetEq(addPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(addPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(addPZ(x,y,z)) );
   }

   void testSub()
   {
      Range x(-2,5), y(-8,-1), z(-6,4), f(-1,13),
            px(-2,3), py(-6,-1), pz(-1,4);
      TEST_TRUE( f.isSetEq(x - y) );
      TEST_TRUE( px.isSetEq(subPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(subPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(subPZ(x,y,z)) );
   }

   void testMulA()
   {
      Range x(1,3), y(-3,-1), z(-2,2), f(-9,-1),
            px(1,2), py(-2,-1), pz(-2,-1);
      TEST_TRUE( f.isSetEq(x * y) );
      TEST_TRUE( px.isSetEq(mulPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(mulPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(mulPZ(x,y,z)) );
   }

   void testMulB()
   {
      Range x(-3,4), y(1,7), z(0,50), f(-21,28),
            px(0,4), py(1,7), pz(0,28);
      TEST_TRUE( f.isSetEq(x * y) );
      TEST_TRUE( px.isSetEq(mulPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(mulPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(mulPZ(x,y,z)) );
   }

   void testSqrA()
   {
      Range x(-2,5), y(2,10), f(0,25), px(-2,3), py(2,10);
      TEST_TRUE( f.isSetEq(sqr(x)) );
      TEST_TRUE( px.isSetEq(sqrPX(x,y)) );
      TEST_TRUE( py.isSetEq(sqrPY(x,y)) );
   }

   void testSqrB()
   {
      Range x(-1,4), y(-3,5), f(0,16), px(-1,2), py(0,5);
      TEST_TRUE( f.isSetEq(sqr(x)) );
      TEST_TRUE( px.isSetEq(sqrPX(x,y)) );
      TEST_TRUE( py.isSetEq(sqrPY(x,y)) );
   }

   void testAbsA()
   {
      Range x(3,6), y(2,5), f(3,6), px(3,5), py(3,5);
      TEST_TRUE( f.isSetEq(abs(x)) );
      TEST_TRUE( px.isSetEq(absPX(x,y)) );
      TEST_TRUE( py.isSetEq(absPY(x,y)) );      
   }

   void testAbsB()
   {
      Range x(-6,-3), y(2,5), f(3,6), px(-5,-3), py(3,5);
      TEST_TRUE( f.isSetEq(abs(x)) );
      TEST_TRUE( px.isSetEq(absPX(x,y)) );
      TEST_TRUE( py.isSetEq(absPY(x,y)) );      
   }

   void testMin()
   {
      Range x(0,4), y(-2,8), z(-1,7), f(-2,4),
            px(0,4), py(-1,8), pz(-1,4);
      TEST_TRUE( f.isSetEq(min(x,y)) );
      TEST_TRUE( px.isSetEq(minPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(minPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(minPZ(x,y,z)) );
   }

   void testMax()
   {
      Range x(0,4), y(-2,8), z(-1,7), f(0,8),
            px(0,4), py(-2,7), pz(0,7);
      TEST_TRUE( f.isSetEq(max(x,y)) );
      TEST_TRUE( px.isSetEq(maxPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(maxPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(maxPZ(x,y,z)) );
   }

   void testDivA()
   {
      Range x(1,8), y(2,11), z(3,5), f(1,4),
            px(6,8), py(2,2), pz(3,4);
      TEST_TRUE( f.isSetEq(x / y) );
      TEST_TRUE( px.isSetEq(divPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(divPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(divPZ(x,y,z)) );
   }

   void testDivB()
   {
      Range x(1,4), y(-2,3), z(0,6), f(-4,4),
            px(1,4), py(1,3), pz(1,4);
      TEST_TRUE( f.isSetEq(x / y) );
      TEST_TRUE( px.isSetEq(divPX(x,y,z)) );
      TEST_TRUE( py.isSetEq(divPY(x,y,z)) );
      TEST_TRUE( pz.isSetEq(divPZ(x,y,z)) );
   }

   void testMod()
   {
      int n = 7;
      Range x(12,16), f(0,6);
      
      std::pair<Range,Range> p = extMod(x,n);
      TEST_TRUE( p.first.isSetEq(Range(0,2)) );
      TEST_TRUE( p.second.isSetEq(Range(5,6)) );
      TEST_TRUE( f.isSetEq(x % n) );

      Range y(0,3), px(14,16), py(0,2);
      TEST_TRUE( px.isSetEq(modPX(x,n,y)) );
      TEST_TRUE( py.isSetEq(modPY(x,n,y)) );
   }

   void testWidth()
   {
      Range x(-1,3), y(2);
      TEST_TRUE( x.nbElems() == 5);
      TEST_TRUE( y.nbElems() == 1);
   }
};

REGISTER_TEST(RangeTest)
