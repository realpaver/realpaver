#include "gaol/gaol_interval.h"
#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintProver.hpp"
#include "realpaver/DagExtended.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Prover.hpp"
#include "test_config.hpp"

void test_eq()
{
   Problem pbm;
   Variable x = pbm.addRealVar(0.5, 1.2, "x"), y = pbm.addRealVar(0.8, 1.9, "y");
   pbm.addCtr(sqr(x) + sqr(y) - 2.0 == 0.0);
   pbm.addCtr(y - sqr(x) == 0.0);

   SharedDagExtended edag = std::make_shared<DagExtended>(pbm);
   Prover prover(edag);

   IntervalBox B(pbm.scope()), mid(B.midpoint());
   Proof proof = prover.certify(mid);

   std::cerr << "proof : " << proof << std::endl;
   std::cerr << "mid : " << mid << std::endl;

   TEST_TRUE(proof == Proof::Feasible);
}

int main()
{
   TEST(test_eq)

   END_TEST
}
