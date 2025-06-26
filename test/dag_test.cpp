#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"

Problem *prob;
Dag *dag;

void init()
{
   prob = new Problem();
   dag = new Dag();
   Variable x = prob->addRealVar(0, 0, "x"), y = prob->addRealVar(0, 0, "y"),
            z = prob->addRealVar(0, 0, "z");

   size_t i = prob->addCtr(y - sqr(x) == 0);
   size_t j = prob->addCtr(sqr(x) + sqr(y) == 0);
   size_t k = prob->addCtr(sqr(x) + sqr(y) - z == 0);

   dag->insert(prob->ctrAt(i));
   dag->insert(prob->ctrAt(j));
   dag->insert(prob->ctrAt(k));
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
