#include "test_config.hpp"
#include "realpaver/RealFunctionVector.hpp"
#include "realpaver/Variable.hpp"
#include "realpaver/Dag.hpp"


void test_diff()
{
    Variable x("x"), y("y");
    Term t = pow(x,2) + 2*y;

    SharedDag dag = std::make_shared<Dag>();
    dag->insert(t==0);
    RealFunctionVector rfv(dag);

    RealPoint rp(rfv.scope());
    rp[0] = 2; rp[1] = 2;

    RealVector rv(rfv.nbFuns());
    rfv.eval(rp,rv);
    RealMatrix jac(rfv.nbFuns(),rfv.nbVars());
    rfv.diff(rp,jac);

    TEST_TRUE(rv[0] == 8);
    TEST_TRUE(jac.get(0,0) == 4);
    TEST_TRUE(jac.get(0,1) == 2);
}

int main()
{
   TEST(test_diff)

   END_TEST
}
