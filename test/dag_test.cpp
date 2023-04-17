#include "test_config.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"

Problem* prob;
Dag* dag;

void init()
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

void clean()
{
   delete dag;
   delete prob;
}

void test_1()
{
   TEST_TRUE(dag->nbVars() == 3);
   TEST_TRUE(dag->nbNodes() == 8);
}

void test_2()
{
   TEST_TRUE(dag->varNode(0)->parArity() == 1);
   TEST_TRUE(dag->varNode(1)->parArity() == 2);
   TEST_TRUE(dag->varNode(2)->parArity() == 1);
}

int main()
{
   INIT_TEST

   TEST(test_1)
   TEST(test_2)

   CLEAN_TEST
   END_TEST
}
