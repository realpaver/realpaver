#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver_test.hpp"

class Hc4ReviseTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( Hc4ReviseTest );
   CPPUNIT_TEST( testA );
   CPPUNIT_TEST( testB );
   CPPUNIT_TEST( testC );
   CPPUNIT_TEST( testD );
   CPPUNIT_TEST_SUITE_END();

   Problem* prob;
   Dag* dag;
   IntervalRegion* reg;
   Variable x, y, z;

public:
   void setUp()
   {
      prob = new Problem();
      x = prob->addRealVar(0, 0, "x");
      y = prob->addRealVar(0, 0, "y");
      z = prob->addRealVar(0, 0, "z");

      dag = new Dag();
      dag->insert( sqr(x + y) - 2*z + 2 == 0 );
      dag->insert( sqr(x + y) - 2*z + 2 >= 0 );

      reg = new IntervalRegion(prob->getDomains());
   }

   void tearDown()
   {
      delete reg;
      delete dag;
      delete prob;
   }

   void testA()
   {
      reg->set(x, Interval(-10, 15));
      reg->set(y, Interval(-20, 5));
      reg->set(z, Interval(-10, 5.5));

      Proof p = dag->fun(0)->hc4Revise(*reg);

      TEST_TRUE( p == Proof::Maybe );
      TEST_TRUE( reg->get(x).isSetEq(Interval(-8, 15)) );
      TEST_TRUE( reg->get(y).isSetEq(Interval(-18, 5)) );
      TEST_TRUE( reg->get(z).isSetEq(Interval(1, 5.5)) );
   }

   void testB()
   {
      reg->set(x, Interval::universe());
      reg->set(y, Interval(-20, 5));
      reg->set(z, Interval(-10, 5.5));

      Proof p = dag->fun(0)->hc4Revise(*reg);

      TEST_TRUE( p == Proof::Maybe );
      TEST_TRUE( reg->get(x).isSetEq(Interval(-8, 23)) );
      TEST_TRUE( reg->get(y).isSetEq(Interval(-20, 5)) );
      TEST_TRUE( reg->get(z).isSetEq(Interval(1, 5.5)) );
   }

   void testC()
   {
      reg->set(x, Interval(-10, 15));
      reg->set(y, Interval(-20, 5));
      reg->set(z, Interval(-10, 0));

      Proof p = dag->fun(0)->hc4Revise(*reg);

      TEST_TRUE( p == Proof::Empty );
   }

   void testD()
   {
      reg->set(x, Interval(2, 4));
      reg->set(y, Interval(3, 10));
      reg->set(z, Interval(0, 6));

      Proof p = dag->fun(1)->hc4Revise(*reg);

      TEST_TRUE( p == Proof::Inner );
   }

};

REGISTER_TEST(Hc4ReviseTest)
