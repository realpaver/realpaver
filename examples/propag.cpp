#include "realpaver_api.hpp"
#include <iostream>

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::full, "propag.log");

   try
   {
      Problem problem;

      Variable x = problem.addRealVar(-7, 3, "x"), y = problem.addRealVar(-3, 6, "y");
      size_t i1 = problem.addCtr(y - sqr(x) == 0),
             i2 = problem.addCtr(sqr(x) + sqr(y) - 2.0 == 0);

      DomainBox dbox(problem.scope());
      cout << "Box:    " << dbox << endl;

      IntervalBox B(dbox);
      cout << "Interval box:     " << B << endl;

      SharedDag dag = std::make_shared<Dag>();
      size_t j1 = dag->insert(problem.ctrAt(i1));
      size_t j2 = dag->insert(problem.ctrAt(i2));

      cout << "-- DAG:" << endl << (*dag) << "--" << endl;

      SharedContractorPool pool = std::make_shared<ContractorPool>();
      pool->push(std::make_shared<ContractorHC4Revise>(dag, j1));
      pool->push(std::make_shared<ContractorHC4Revise>(dag, j2));

      IntervalPropagator tor(pool);
      tor.setTol(1.0e-4);

      Proof proof = tor.contract(B);

      cout << "Proof:      " << proof << endl;

      if (proof != Proof::Empty)
         cout << "New box: " << B << endl;
   }
   catch (Exception e)
   {
      cout << e.what() << endl;
   }

   return 0;
}
