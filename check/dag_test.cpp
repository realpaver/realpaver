#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver_test.hpp"

class DagTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( DagTest );
   CPPUNIT_TEST( testA );
   CPPUNIT_TEST( testB );
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

      dag->insert( y - sqr(x) == 0 );
      dag->insert( sqr(x) + sqr(y) == 0 );
      dag->insert( sqr(x) + sqr(y) - z == 0 );
   }

   void tearDown()
   {
      delete dag;
      delete prob;
   }

   void testA()
   {
      TEST_TRUE( dag->nbVar() == 3 );
      TEST_TRUE( dag->nbNode() == 8 );
   }

   void testB()
   {
      TEST_TRUE( dag->varNode(0)->parArity() == 1 );
      TEST_TRUE( dag->varNode(1)->parArity() == 2 );
      TEST_TRUE( dag->varNode(2)->parArity() == 1 );
   }

};

REGISTER_TEST(DagTest)
