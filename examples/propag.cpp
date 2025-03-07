#include <iostream>
#include "realpaver_api.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::full, "propag.log");

   try {
      Problem problem;

      Variable x = problem.addRealVar(-7, 3, "x"),
               y = problem.addRealVar(-3, 6, "y");

      DomainBox dbox(problem.scope());
      cout << "Box:    " << dbox << endl;

      IntervalBox B(dbox);
      cout << "Interval box:     " << B << endl;

      SharedDag dag = std::make_shared<Dag>();
      //~ size_t i = dag->insert( y == 0.5*x );
      //~ size_t j = dag->insert( y == 2*x );

      size_t i = dag->insert( y - sqr(x) == 0 );
      size_t j = dag->insert( sqr(x) + sqr(y) - 2.0 == 0 );

      cout << (*dag) << endl;

      SharedContractorPool pool = std::make_shared<ContractorPool>();
      pool->push(std::make_shared<ContractorHC4Revise>(dag, i));
      pool->push(std::make_shared<ContractorHC4Revise>(dag, j));

      IntervalPropagator tor(pool);
      tor.setTol(1.0e-4);

      Proof proof = tor.contract(B);

      cout << "Proof:      " << proof << endl;

      if (proof != Proof::Empty)
         cout << "New box: " << B << endl;
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
