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
   Logger::init(LogLevel::internal, "bop.log");
   Interval::precision( 8 );

   try {
      Problem problem;

      Variable x = problem.addRealVar(-10,  4, "x"),
               y = problem.addRealVar( -3,  7, "y");

      IntervalVector V = problem.getDomains();

      cout << "Region:     " << V << endl;

      Dag dag;
      size_t i = dag.insert( y == 0.5*x );
      size_t j = dag.insert( y == 2*x );

      ContractorVector pool;
      pool.push(std::make_shared<HC4Contractor>(&dag, i));
      pool.push(std::make_shared<HC4Contractor>(&dag, j));

      Propagator tor(&pool);
      tor.setDistTol(Tolerance::makeRel(1.0e-2));

      Proof proof = tor.contract(V);

      cout << "Proof:      " << proof << endl;

      if (proof != Proof::Empty)
         cout << "New region: " << V << endl;      

   }
   catch(Exception e) {
      cout << e.what() << endl;
   }

   return 0;
}
