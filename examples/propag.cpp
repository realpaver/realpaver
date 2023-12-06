#include <iostream>
#include "realpaver/HC4Contractor.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Propagator.hpp"

using namespace realpaver;
using namespace std;

int main(void)
{
   Logger::init(LogLevel::full, "propag.log");

   try {
      Problem problem;

      Variable x = problem.addRealVar(-7,  3, "x"),
               y = problem.addRealVar( -3, 6, "y");

      IntervalRegion reg = problem.makeIntervalRegion();

      cout << "Region:     " << reg << endl;

      SharedDag dag = std::make_shared<Dag>();
      //~ size_t i = dag->insert( y == 0.5*x );
      //~ size_t j = dag->insert( y == 2*x );

      size_t i = dag->insert( y - sqr(x) == 0 );
      size_t j = dag->insert( sqr(x) + sqr(y) - 2.0 == 0 );

      cout << (*dag) << endl;

      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(std::make_shared<HC4Contractor>(dag, i));
      pool->push(std::make_shared<HC4Contractor>(dag, j));

      Propagator tor(pool);
      tor.setDistTol(Tolerance::makeRel(1.0e-2));

      Proof proof = tor.contract(reg);

      cout << "Proof:      " << proof << endl;

      if (proof != Proof::Empty)
         cout << "New region: " << reg << endl;      
   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
