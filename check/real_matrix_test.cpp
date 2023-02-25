#include "realpaver/RealMatrix.hpp"
#include "realpaver_test.hpp"

class RealMatrixTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( RealMatrixTest );
   CPPUNIT_TEST( testInv1 );
   CPPUNIT_TEST( testInv2 );
   CPPUNIT_TEST( testInv3 );
   CPPUNIT_TEST( testInv4 );
   CPPUNIT_TEST_SUITE_END();

public:
   void setUp()
   {}

   void tearDown()
   {}

   void testInv1()
   {
      RealMatrix M({{1, -1},
                    {-2, 1}});

      RealMatrix Q({{-1, -1},
                    {-2, -1}});

      RealMatrix P(2, 2);
      bool b = M.inverse(P);

      TEST_TRUE( b == true );
      TEST_TRUE( P == Q );
   }

   void testInv2()
   {
      RealMatrix M({{1, 1, 1},
                    {1, 2, 2},
                    {1, 2, 3}});

      RealMatrix Q({{2, -1, 0},
                    {-1, 2, -1},
                    {0, -1, 1}});

      RealMatrix P(3, 3);
      bool b = M.inverse(P);

      TEST_TRUE( b == true );
      TEST_TRUE( P == Q );
   }

   void testInv3()
   {
      RealMatrix M({{1, -1},
                    {2, -2}});

      RealMatrix P(2, 2);
      bool b = M.inverse(P);

      TEST_TRUE( b == false );
   }

   void testInv4()
   {
      RealMatrix M({{1, -1, 2},
                    {0, 1, 4},
                    {1, 2, 14}});

      RealMatrix P(3, 3);
      bool b = M.inverse(P);

      TEST_TRUE( b == false );
   }

};

REGISTER_TEST(RealMatrixTest)
