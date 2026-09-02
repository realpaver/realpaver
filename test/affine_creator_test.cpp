#include "realpaver/AffineCreator.hpp"
#include "realpaver/Problem.hpp"
#include "test_config.hpp"

void init()
{
}

void clean()
{
}

// void test_single_occ()
// {
//    Problem P;
//    Variable x = P.addRealVar(-1, 3, "x"), y = P.addRealVar(-2, 2, "y");
//    IntervalBox B(P.scope());
//    size_t i = P.addCtr(sqr(x) + sqr(y) == 2.0);
//    size_t j = P.addCtr(y - sqr(x) == 0.0);

//    SharedDag dag = std::make_shared<Dag>();
//    dag->insert(P.ctrAt(i));
//    dag->insert(P.ctrAt(j));

//    AffineCreator creator(dag);
//    Bitset active(dag->nbFuns());
//    active.setAllOne();
//    creator.create(B, active);

//    AffineForm f(3.0, {4.0}, {0}, 4.0), g(-3.0, {-4.0, 2.0}, {0, 1}, 2.0);

//    TEST_TRUE(f.equals(creator.fun(0)));
//    TEST_TRUE(g.equals(creator.fun(1)));

//    TEST_TRUE(creator.fun(0).eval().contains(dag->fun(0)->iEval(B)));
//    TEST_TRUE(creator.fun(1).eval().contains(dag->fun(1)->iEval(B)));
// }

// void test_lin()
// {
//    Problem P;
//    Variable x = P.addRealVar(-1, 3, "x"), y = P.addRealVar(1, 6, "y");
//    IntervalBox B(P.scope());
//    size_t i = P.addCtr(1.5 * x - 2.5 * y - 2.0 == 0);

//    SharedDag dag = std::make_shared<Dag>();
//    dag->insert(P.ctrAt(i));

//    AffineCreator creator(dag);
//    creator.create(B, 0);

//    AffineForm f(-9.25, {3.0, -6.25}, {0, 1});
//    TEST_TRUE(f.equals(creator.fun(0)));
//    TEST_TRUE(creator.fun(0).eval().isSetEq(dag->fun(0)->iEval(B)));
// }

// void test_subset()
// {
//    Problem P;
//    Variable x = P.addRealVar(-1, 3, "x"), y = P.addRealVar(-2, 2, "y");
//    IntervalBox B(P.scope());
//    size_t i = P.addCtr(sqr(x) + sqr(y) == 2.0);
//    size_t j = P.addCtr(1.5 * x - 2.5 * y - 2.0 == 0);
//    size_t k = P.addCtr(y - sqr(x) == 0.0);

//    SharedDag dag = std::make_shared<Dag>();
//    dag->insert(P.ctrAt(i));
//    dag->insert(P.ctrAt(j));
//    dag->insert(P.ctrAt(k));

//    Bitset active({1, 0, 1});
//    AffineCreator creator(dag);
//    creator.create(B, active);

//    AffineForm f(3.0, {4.0}, {0}, 4.0), g(-3.0, {-4.0, 2.0}, {0, 1}, 2.0);

//    TEST_TRUE(f.equals(creator.fun(0)));
//    TEST_TRUE(g.equals(creator.fun(1)));
// }
// TODO

int main()
{
   INIT_TEST

   // TODO
   // TEST(test_single_occ)
   // TEST(test_lin)
   // TEST(test_subset)

   CLEAN_TEST
   END_TEST
}
