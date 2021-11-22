#include "realpaver_dag.hpp"
#include "realpaver_problem.hpp"
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

public:
   void setUp()
   {
      prob = new Problem();
      dag = new Dag();
      Variable x = prob->addRealVar(0, 0, "x"),
               y = prob->addRealVar(0, 0, "y"),
               z = prob->addRealVar(0, 0, "z");
      dag->insert( sqr(x + y) - 2*z + 2 == 0 );
      dag->insert( sqr(x + y) - 2*z + 2 >= 0 );
   }

   void tearDown()
   {
      delete dag;
      delete prob;
   }

   void testA()
   {
      IntervalVector V = { Interval(-10, 15),
                           Interval(-20, 5),
                           Interval(-10, 5.5) };

      Proof p = dag->fun(0)->hc4Revise(V);

      TEST_TRUE( p == Proof::Maybe );
      TEST_TRUE( V[0].isSetEq(Interval(-8, 15)) );
      TEST_TRUE( V[1].isSetEq(Interval(-18, 5)) );
      TEST_TRUE( V[2].isSetEq(Interval(1, 5.5)) );
   }

   void testB()
   {
      IntervalVector V = { Interval::universe(),
                           Interval(-20, 5),
                           Interval(-10, 5.5) };

      Proof p = dag->fun(0)->hc4Revise(V);

      TEST_TRUE( p == Proof::Maybe );
      TEST_TRUE( V[0].isSetEq(Interval(-8, 23)) );
      TEST_TRUE( V[1].isSetEq(Interval(-20, 5)) );
      TEST_TRUE( V[2].isSetEq(Interval(1, 5.5)) );
   }

   void testC()
   {
      IntervalVector V = { Interval(-10, 15),
                           Interval(-20, 5),
                           Interval(-10, 0) };

      Proof p = dag->fun(0)->hc4Revise(V);

      TEST_TRUE( p == Proof::Empty );
   }

   void testD()
   {
      IntervalVector V = { Interval(2, 4),
                           Interval(3, 10),
                           Interval(0, 6) };

      Proof p = dag->fun(1)->hc4Revise(V);

      TEST_TRUE( p == Proof::Inner );
   }

};

REGISTER_TEST(Hc4ReviseTest)
