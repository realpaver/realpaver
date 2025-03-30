#include "test_config.hpp"
#include "realpaver/AffineCreator.hpp"
#include "realpaver/Problem.hpp"

Problem* P;
IntervalBox* B;
Variable x, y, z;

void init()
{
   P = new Problem();
   x = P->addRealVar(-10, 10, "x");
   y = P->addRealVar(-10, 10, "y");
   z = P->addRealVar(-10, 10, "z");
   B = new IntervalBox(P->scope());
}

void clean()
{
   delete B;
   delete P;
}

void test_single_occ()
{
   B->set(x, Interval(-1, 3));
   B->set(y, Interval(-2, 2));

   SharedDag dag = std::make_shared<Dag>();
   dag->insert(sqr(x) + sqr(y) == 2.0);
   dag->insert(y - sqr(x) == 0.0);

   AffineCreator creator(dag);
   creator.create(*B);

   AffineForm f(3.0, {4.0}, {0}, 4.0),
              g(-3.0, {-4.0, 2.0}, {0, 1}, 2.0);

   TEST_TRUE(f.equals(creator.fun(0)));
   TEST_TRUE(g.equals(creator.fun(1)));

   TEST_TRUE(creator.fun(0).eval().contains(dag->fun(0)->iEval(*B)));
   TEST_TRUE(creator.fun(1).eval().contains(dag->fun(1)->iEval(*B)));
}

void test_lin()
{
   B->set(x, Interval(-1, 3));
   B->set(y, Interval(1, 6));

   SharedDag dag = std::make_shared<Dag>();
   dag->insert(1.5*x - 2.5*y - 2.0 == 0);

   AffineCreator creator(dag);
   creator.create(*B);

   AffineForm f(-9.25, {3.0, -6.25}, {0, 1});
   TEST_TRUE(f.equals(creator.fun(0)));
   TEST_TRUE(creator.fun(0).eval().isSetEq(dag->fun(0)->iEval(*B)));
}

void test_subset()
{
   B->set(x, Interval(-1, 3));
   B->set(y, Interval(-2, 2));

   SharedDag dag = std::make_shared<Dag>();
   dag->insert(sqr(x) + sqr(y) == 2.0);
   dag->insert(1.5*x - 2.5*y - 2.0 == 0);
   dag->insert(y - sqr(x) == 0.0);

   std::vector<size_t> lfun = {0, 2};
   AffineCreator creator(dag, lfun);
   creator.create(*B);

   AffineForm f(3.0, {4.0}, {0}, 4.0),
              g(-3.0, {-4.0, 2.0}, {0, 1}, 2.0);

   TEST_TRUE(f.equals(creator.fun(0)));
   TEST_TRUE(g.equals(creator.fun(1)));
}

int main()
{
   INIT_TEST

   TEST(test_single_occ)
   TEST(test_lin)
   TEST(test_subset)

   CLEAN_TEST
   END_TEST
}
