#include "realpaver/ContractorAffineRevise.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"

Problem *prob;
SharedDag dag;
IntervalBox *B;
Variable x, y, z;

void init()
{
   prob = new Problem();
   x = prob->addRealVar(0, 0, "x");
   y = prob->addRealVar(0, 0, "y");
   z = prob->addRealVar(0, 0, "z");

   dag = std::make_shared<Dag>();
   dag->insert(2 * x - y + z - 1 == 0);
   dag->insert(2 * x - y + z - 1 >= 0);
   dag->insert(2 * x + sqr(y) + z == 0);

   B = new IntervalBox(prob->scope());
}

void clean()
{
   delete B;
   delete prob;
}

void test_1()
{
   B->set(x, Interval(1, 7));
   B->set(y, Interval(-1, 2));
   B->set(z, Interval(-1, 3));

   ContractorAffineRevise ctc(dag, 0, true);
   Proof p = ctc.contract(*B);

   TEST_TRUE(p == Proof::Maybe);

   Interval rx(1, 2), ry(0, 2), rz(-1, 1);

   TEST_TRUE(B->get(x).contains(rx));
   TEST_TRUE(B->get(y).contains(ry));
   TEST_TRUE(B->get(z).contains(rz));

   TEST_TRUE(B->get(x).distance(rx) <= 1.0e-12);
   TEST_TRUE(B->get(y).distance(ry) <= 1.0e-12);
   TEST_TRUE(B->get(z).distance(rz) <= 1.0e-12);
}

void test_2()
{
   B->set(x, Interval(10, 13));
   B->set(y, Interval(-1, 2));
   B->set(z, Interval(-1, 3));

   ContractorAffineRevise ctc(dag, 0, true);
   Proof p = ctc.contract(*B);

   TEST_TRUE(p == Proof::Empty);
}

void test_3()
{
   B->set(x, Interval(10, 13));
   B->set(y, Interval(-1, 2));
   B->set(z, Interval(-1, 3));

   ContractorAffineRevise ctc(dag, 1, true);
   Proof p = ctc.contract(*B);

   TEST_TRUE(p == Proof::Inner);
}

void test_4()
{
   B->set(x, Interval(-13, -10));
   B->set(y, Interval(-1, 2));
   B->set(z, Interval(-1, 3));

   ContractorAffineRevise ctc(dag, 1, true);
   Proof p = ctc.contract(*B);

   TEST_TRUE(p == Proof::Empty);
}

void test_5()
{
   B->set(x, Interval(0, 7));
   B->set(y, Interval(-1, 2));
   B->set(z, Interval(-1, 3));

   ContractorAffineRevise ctc(dag, 2, true);
   Proof p = ctc.contract(*B);

   TEST_TRUE(p == Proof::Maybe);

   Interval hx(0, 0.5), hy(-1, 1), hz(-1, 0);
   Interval rx(0, 1.125), ry(-1, 1.25), rz(-1, 1.25);

   TEST_TRUE(B->get(x).contains(hx));
   TEST_TRUE(B->get(y).contains(hy));
   TEST_TRUE(B->get(z).contains(hz));

   TEST_TRUE(B->get(x).distance(rx) <= 1.0e-12);
   TEST_TRUE(B->get(y).distance(ry) <= 1.0e-12);
   TEST_TRUE(B->get(z).distance(rz) <= 1.0e-12);
}

int main()
{
   INIT_TEST

   TEST(test_1)
   TEST(test_2)
   TEST(test_3)
   TEST(test_4)
   TEST(test_5)

   CLEAN_TEST
   END_TEST
}
